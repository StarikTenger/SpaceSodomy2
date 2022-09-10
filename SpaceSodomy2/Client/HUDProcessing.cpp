#include "HUDProcessing.h"

bool operator<(Player& a, Player& b) {
	return ((a.get_kills() > b.get_kills() ||
		(a.get_kills() == b.get_kills() && a.get_deaths() < b.get_deaths())));
}

void HUDProcessing::get_buttons_names(tgui::Gui &gui, std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	for (int i = 0; !config.eof(); i++) {
		std::string cur, cur_name;
		config >> cur_name >> cur;
		if (cur_name == "RESPAWN") {
			respawn_button_name = cur;
		}
		if (cur_name == "BONUS_ACTIVATION") {
			bonus_button_name = cur;
			gui.get<tgui::Label>("BonusTip")->setText(cur);
		}
		if (cur_name == "LEFT_MODULE") {
			left_module_button_name = cur;
			gui.get<tgui::Label>("LeftModuleTip")->setText(cur);
		}
		if (cur_name == "RIGHT_MODULE") {
			right_module_button_name = cur;
			gui.get<tgui::Label>("RightModuleTip")->setText(cur);
		}
		for (int j = 0; !config.eof() && (cur != "END"); j++) {
			config >> cur;
		}
	}
}

std::vector<b2Vec2> HUDProcessing::get_vertices(float cur_pos, b2Vec2 center, b2Vec2 scale) {
	std::vector<b2Vec2> ans;
	if (cur_pos < b2_epsilon)
		return ans;
	ans.push_back(center);
	center -= b2Vec2(0, scale.y / 2.0);
	ans.push_back(center);
	if (cur_pos < 0.125) {
		center -= b2Vec2(scale.x / 2.0 * (cur_pos / 0.125), 0);
		ans.push_back(center);
		return ans;
	}
	else {
		center -= b2Vec2(scale.x / 2.0, 0);
		cur_pos -= 0.125;
		ans.push_back(center);
	}
	if (cur_pos < 0.25) {
		center += b2Vec2(0, scale.y * (cur_pos / 0.25));
		ans.push_back(center);
		return ans;
	}
	else {
		center += b2Vec2(0, scale.y);
		cur_pos -= 0.25;
		ans.push_back(center);
	}
	if (cur_pos < 0.25) {
		center += b2Vec2(scale.x * (cur_pos / 0.25), 0);
		ans.push_back(center);
		return ans;
	}
	else {
		center += b2Vec2(scale.x, 0);
		cur_pos -= 0.25;
		ans.push_back(center);
	}
	if (cur_pos < 0.25) {
		center -= b2Vec2(0, scale.y * (cur_pos / 0.25));
		ans.push_back(center);
		return ans;
	}
	else {
		center -= b2Vec2(0, scale.y);
		cur_pos -= 0.25;
		ans.push_back(center);
	}
	center -= b2Vec2(scale.x / 2.0 * (cur_pos / 0.125), 0);
	ans.push_back(center);
	return ans;
}

void HUDProcessing::redraw_shadow(tgui::CanvasSFML::Ptr canvas, tgui::Picture::Ptr pic) {
	canvas->setSize(pic->getSize());
	canvas->setPosition(pic->getPosition());

	canvas->clear(sf::Color(0, 0, 0, 0));

	auto fill_polygon = [](std::vector<b2Vec2> vertices, sf::Color color) {
		sf::ConvexShape convex;

		convex.setPointCount(vertices.size());
		convex.setFillColor(color);

		for (int i = 0; i < vertices.size(); i++) {
			convex.setPoint(i, aux::to_Vector2f(vertices[i]));
		}
		return convex;
	};

	if (pic->getWidgetName() == "LeftModule")
		canvas->draw(fill_polygon(get_vertices(game->get_ship(player_network->get_id())->get_left_module()->get_recharge_counter()->get() /
			game->get_ship(player_network->get_id())->get_left_module()->get_recharge_counter()->get_max(),
			aux::to_b2Vec2(pic->getSize() * 0.5), aux::to_b2Vec2(pic->getSize())), sf::Color(0, 0, 0, 150)));
	else
		canvas->draw(fill_polygon(get_vertices(game->get_ship(player_network->get_id())->get_right_module()->get_recharge_counter()->get() /
			game->get_ship(player_network->get_id())->get_right_module()->get_recharge_counter()->get_max(),
			aux::to_b2Vec2(pic->getSize() * 0.5), aux::to_b2Vec2(pic->getSize())), sf::Color(0, 0, 0, 150)));

	canvas->display();
}

void HUDProcessing::table_step(tgui::Gui &gui, float scale) {
	// Init player array and fill it with players, than sort
	std::vector<Player> rating_table;
	for (auto player : *game->get_players()) {
		if (player.second == nullptr)
			continue;
		rating_table.push_back(*player.second);
	}
	sort(rating_table.begin(), rating_table.end());

	// Clean up rating table
	gui.get<tgui::Group>("RatingTable")->removeAllWidgets();

	// Applies scale & design to label text
	auto apply_label = [=](std::vector<Player> *rating_table, tgui::Label::Ptr& label, int num) {
		label->getRenderer()->setTextOutlineColor("black");
		label->getRenderer()->setTextOutlineThickness(2);
		label->getRenderer()->setTextColor(rating_table->operator[](num).get_color());
		label->setTextSize(7 * scale);
		label->setScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	};

	// Fill table with labels
	int players_to_display = 5; // How many players (excluding you) we want to see in the table
	int line_interval = 12; // Interval between neighbor lines
	for (int i = 0; i < rating_table.size(); i++) {
		// Check for players_to_display
		if (rating_table[i].get_name() != player_network->get_name() && i > players_to_display - 1)
			continue;
		// Line in wich label is displayed
		int line_number = std::min(i, players_to_display);

		// Create name label
		auto name = tgui::Label::create();
		apply_label(&rating_table, name, i);
		name->setPosition(tgui::Layout2d(0, tgui::String(std::to_string(5 + line_interval * line_number) + "%")));
		name->setSize(tgui::Layout2d("50%", "15%"));
		name->setText(std::to_string(i + 1) + "." + rating_table[i].get_name());

		// Create kills label
		auto kills = tgui::Label::create();
		apply_label(&rating_table, kills, i);
		kills->setPosition(tgui::Layout2d("50%", tgui::String(std::to_string(5 + line_interval * line_number) + "%")));
		kills->setSize(tgui::Layout2d("25%", "15%"));
		kills->setText(std::to_string(rating_table[i].get_kills()));

		// Create deaths label
		auto deaths = tgui::Label::create();
		apply_label(&rating_table, deaths, i);
		deaths->setPosition(tgui::Layout2d("75%", tgui::String(std::to_string(5 + line_interval * line_number) + "%")));
		deaths->setSize(tgui::Layout2d("25%", "15%"));
		deaths->setText(std::to_string(rating_table[i].get_deaths()));
		
		// Add labels to table
		gui.get<tgui::Group>("RatingTable")->add(name);
		gui.get<tgui::Group>("RatingTable")->add(kills);
		gui.get<tgui::Group>("RatingTable")->add(deaths);
	}
}

HUDProcessing::HUDProcessing()
{
}

HUDProcessing::HUDProcessing(tgui::Gui &gui, Draw* draw_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_,
	GameClient* game_, ClientNetwork* player_network_, std::queue<int>* frame_marks_) {
	frame_marks = frame_marks_;
	game = game_;
	player_network = player_network_;
	draw = draw_;
	mouse_pos = mouse_pos_;
	keyboard = keyboard_;
	hcolor = gui.get<tgui::ProgressBar>("HPBar")->getRenderer()->getFillColor();
	scolor = gui.get<tgui::ProgressBar>("StaminaBar")->getRenderer()->getFillColor();
	ecolor = gui.get<tgui::ProgressBar>("EnergyBar")->getRenderer()->getFillColor();
}

void HUDProcessing::step(tgui::Gui &gui) {
	get_buttons_names(gui, "keys.conf");
	sf::Vector2f win_s = 
		sf::Vector2f(1.0 * draw->get_window()->getSize().x / sf::VideoMode::getDesktopMode().width,
			1.0 * draw->get_window()->getSize().y / sf::VideoMode::getDesktopMode().height);
	float scale = interface_scale * (win_s.x + win_s.y) / 2.0;
	auto interface_group = gui.get<tgui::Group>("InterfaceGroup");
	interface_group->setSize(tgui::Layout2d(330 * scale, 40 * scale));
	interface_group->setPosition(tgui::Layout2d("50%", "100%") 
		- tgui::Layout2d(330 * scale / 2.0, 40 * scale));	
	gui.get<tgui::Label>("BonusTip")->setTextSize(12.5 * scale);
	gui.get<tgui::Label>("LeftModuleTip")->setTextSize(12.5 * scale);
	gui.get<tgui::Label>("RightModuleTip")->setTextSize(12.5 * scale);
	//auto text_style = gui.get<tgui::Label>("RightModuleTip")->getRenderer()->getTextStyle
	if (game->get_ship(player_network->get_id()) != nullptr) {
		float mod = abs(sin(float(aux::get_milli_count()) / 500));
		// HP bar processing
		auto HP_bar = gui.get<tgui::ProgressBar>("HPBar");
		HP_bar->setMaximum(game->get_ship(player_network->get_id())->get_hp()->get_max());
		HP_bar->setValue(game->get_ship(player_network->get_id())->get_hp()->get());
		gui.get<tgui::Label>("HPBarVal")->setText(std::to_string(int(game->get_ship(player_network->get_id())->get_hp()->get())));
		if (int(game->get_ship(player_network->get_id())->get_hp()->get_max() * 0.3) >= game->get_ship(player_network->get_id())->get_hp()->get()) {
			//HP_bar->setInheritedOpacity(mod);
			HP_bar->getRenderer()->setFillColor(
				tgui::Color(hcolor.r * (1.0 - mod) + 255.0 * mod, hcolor.g * (1.0 - mod) + 255.0 * mod,
					hcolor.b * (1.0 - mod) + 255.0 * mod, hcolor.a));
		}
		else {
			//HP_bar->setInheritedOpacity(1);
			HP_bar->getRenderer()->setFillColor(tgui::Color(hcolor.r, hcolor.g, hcolor.b, hcolor.a));
		}
		// stamina bar processing
		auto stamina_bar = gui.get<tgui::ProgressBar>("StaminaBar");
		stamina_bar->setMaximum(game->get_ship(player_network->get_id())->get_stamina()->get_max());
		stamina_bar->setValue(game->get_ship(player_network->get_id())->get_stamina()->get());
		gui.get<tgui::Label>("StaminaBarVal")->setText(std::to_string(int(game->get_ship(player_network->get_id())->get_stamina()->get())));
		if (int(game->get_ship(player_network->get_id())->get_stamina()->get_max() * 0.3) >= game->get_ship(player_network->get_id())->get_stamina()->get()) {
			//stamina_bar->setInheritedOpacity(mod);
			stamina_bar->getRenderer()->setFillColor(
				tgui::Color(scolor.r * (1.0 - mod) + 255.0 * mod, scolor.g * (1.0 - mod) + 255.0 * mod,
					scolor.b * (1.0 - mod) + 255.0 * mod, scolor.a));
		}
		else {
			//stamina_bar->setInheritedOpacity(1);
			stamina_bar->getRenderer()->setFillColor(
				tgui::Color(scolor.r, scolor.g, scolor.b, scolor.a));
		}
		// energy bar processing
		auto energy_bar = gui.get<tgui::ProgressBar>("EnergyBar");
		energy_bar->setMaximum(game->get_ship(player_network->get_id())->get_energy()->get_max());
		energy_bar->setValue(game->get_ship(player_network->get_id())->get_energy()->get());
		gui.get<tgui::Label>("EnergyBarVal")->setText(std::to_string(int(game->get_ship(player_network->get_id())->get_energy()->get())));
		if (int(game->get_ship(player_network->get_id())->get_energy()->get_max() * 0.3) >= game->get_ship(player_network->get_id())->get_energy()->get()) {
			//energy_bar->setInheritedOpacity(mod);
			energy_bar->getRenderer()->setFillColor(
				tgui::Color(ecolor.r * (1.0 - mod) + 255.0 * mod, ecolor.g * (1.0 - mod) + 255.0 * mod,
					ecolor.b * (1.0 - mod) + 255.0 * mod, ecolor.a));
		}
		else {
			//energy_bar->setInheritedOpacity(1);
			energy_bar->getRenderer()->setFillColor(
				tgui::Color(ecolor.r, ecolor.g, ecolor.b, ecolor.a));
		}

		auto bonus_texture_name = game->get_bonus_texture_name(game->get_ship(player_network->get_id())->get_bonus_slot()->get_current_bonus());
		if (bonus_texture_name != bonus_icon_name) {
			bonus_icon_name = bonus_texture_name;
			auto bonus_texture = draw->get_texture(bonus_texture_name);
			gui.get<tgui::Picture>("Bonus")->getRenderer()->setTexture(*bonus_texture);
		}  

		// interface buttons processing
		auto pic = gui.get<tgui::Picture>("LeftModule");
		auto shadow = gui.get<tgui::CanvasSFML>("LeftModule-shadow");
		if (shadow == nullptr) {
			auto canvas = tgui::CanvasSFML::create(pic->getSize());
			pic->getParent()->add(canvas, pic->getWidgetName() + "-shadow");
			shadow = gui.get<tgui::CanvasSFML>("LeftModule-shadow");
		}
		redraw_shadow(shadow, pic);

		pic = gui.get<tgui::Picture>("RightModule");
		shadow = gui.get<tgui::CanvasSFML>("RightModule-shadow");
		if (shadow == nullptr) {
			auto canvas = tgui::CanvasSFML::create(pic->getSize());
			pic->getParent()->add(canvas, pic->getWidgetName() + "-shadow");
			shadow = gui.get<tgui::CanvasSFML>("RightModule-shadow");
		}
		redraw_shadow(shadow, pic);
		 

		if (reload_icons) {
			reload_icons = 0;
			auto gun_texture_name = game->player_by_id(player_network->get_id())->get_gun_name() + "-gun";
			auto gun_texture = draw->get_texture(gun_texture_name);
			DEBUG_PRINT(game->player_by_id(player_network->get_id())->get_gun_name() + "\"");
			gui.get<tgui::Picture>("GunImage")->getRenderer()->setTexture(*gun_texture);

			auto left_module_texture_name = Module::get_name_by_type(game->get_ship(player_network->get_id())->get_left_module()->get_type()) + "-module";
			auto left_module_texture = draw->get_texture(left_module_texture_name);
			gui.get<tgui::Picture>("LeftModule")->getRenderer()->setTexture(*left_module_texture);
			auto right_module_texture_name = Module::get_name_by_type(game->get_ship(player_network->get_id())->get_right_module()->get_type()) + "-module";
			auto right_module_texture = draw->get_texture(right_module_texture_name);
			gui.get<tgui::Picture>("RightModule")->getRenderer()->setTexture(*right_module_texture);
		}
	}
	else {
		reload_icons = 1;
	}
	auto respawn_info = gui.get<tgui::Label>("RespawnInfo");
	respawn_info->setTextSize(20 * scale);
	if (game->player_by_id(player_network->get_id()) != nullptr) {
		if (!game->player_by_id(player_network->get_id())->get_is_alive()) {
			gui.get<tgui::Group>("InterfaceGroup")->setVisible(false);
			respawn_info->setVisible(true);
			if (game->player_by_id(player_network->get_id())->get_time_to_respawn()->get() > 0) {
				respawn_info->setInheritedOpacity(1);
				respawn_info->setText("Time to respawn: " +
					std::to_string(int(game->player_by_id(player_network->get_id())->get_time_to_respawn()->get())));
			}
			else {
				float mod = abs(sin(float(aux::get_milli_count()) / 500));
				respawn_info->setText("Press " + respawn_button_name + " to respawn");
				respawn_info->setInheritedOpacity(mod);
			}
		}
		else {
			respawn_info->setVisible(false);
			gui.get<tgui::Group>("InterfaceGroup")->setVisible(true);
			if (game->player_by_id(player_network->get_id()) != nullptr) {
				gui.get<tgui::Label>("Ping")->setText(std::to_string(game->player_by_id(player_network->get_id())->get_ping()) + "ms");
			}
			while (!frame_marks->empty() && frame_marks->front() + 1000 < aux::get_milli_count())
				frame_marks->pop();
			gui.get<tgui::Label>("FPS")->setText(std::to_string(frame_marks->size()));
			if (game->get_network_information_active())
				gui.get<tgui::Group>("NetworkInfo")->setVisible(true);
			else
				gui.get<tgui::Group>("NetworkInfo")->setVisible(false);
		}
	}
	gui.get<tgui::Label>("TimeLeftValue")->setText(std::to_string(game->time_left / 60) + ":"  + std::to_string(game->time_left % 60));
	table_step(gui, scale);
}
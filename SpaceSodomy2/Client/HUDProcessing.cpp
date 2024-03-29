#include "HUDProcessing.h"

bool operator<(Player& a, Player& b) {
	return (a.get_kills() > b.get_kills() ||
		(a.get_kills() == b.get_kills() && a.get_deaths() < b.get_deaths()));
}

void HUDProcessing::get_buttons_names(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	for (int i = 0; !config.eof(); i++) {
		std::string cur, cur_name;
		config >> cur_name >> cur;
		if (cur_name == "RESPAWN") {
			respawn_button_name = cur;
			press_r_to_respawn.set_text("Press " + cur + " to respawn");
		}
		if (cur_name == "BONUS_ACTIVATION") {
			bonus_button_name = cur;
			bonus_tip.set_text(cur);
		}
		if (cur_name == "LEFT_MODULE") {
			left_module_button_name = cur;
			left_module_tip.set_text(cur);
		}
		if (cur_name == "RIGHT_MODULE") {
			right_module_button_name = cur;
			right_module_tip.set_text(cur);
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
		center += b2Vec2(scale.x / 2.0 * (cur_pos / 0.125), 0);
		ans.push_back(center);
		return ans;
	}
	else {
		center += b2Vec2(scale.x / 2.0, 0);
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
		center -= b2Vec2(scale.x * (cur_pos / 0.25), 0);
		ans.push_back(center);
		return ans;
	}
	else {
		center -= b2Vec2(scale.x, 0);
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
	center += b2Vec2(scale.x / 2.0 * (cur_pos / 0.125), 0);
	ans.push_back(center);
	return ans;
}

void HUDProcessing::table_step() {
	std::vector<Player> rating_table;
	std::map <int, sf::Color> colors;
	for (auto player : *game->get_players()) {
		if (player.second == nullptr)
			continue;
		rating_table.push_back(*player.second);
	}
	sort(rating_table.begin(), rating_table.end());

	Constant_Text name;
	Constant_Text kills;
	Constant_Text deaths;

	name.set_draw(draw);
	name.set_mouse_pos(mouse_pos);
	name.set_keyboard(keyboard);
	name.set_use_window_cords(table_use_windows_cords);
	name.set_text_character_pixel_size(60);
	name.set_text_scale(table_character_size);

	kills.set_draw(draw);
	kills.set_mouse_pos(mouse_pos);
	kills.set_keyboard(keyboard);
	kills.set_use_window_cords(table_use_windows_cords);
	kills.set_text_character_pixel_size(60);
	kills.set_text_scale(table_character_size);

	deaths.set_draw(draw);
	deaths.set_mouse_pos(mouse_pos);
	deaths.set_keyboard(keyboard);
	deaths.set_use_window_cords(table_use_windows_cords);
	deaths.set_text_character_pixel_size(60);
	deaths.set_text_scale(table_character_size);

	for (int i = 0; i < rating_table.size(); i++) {
		name.set_text(rating_table[i].get_name());
		name.set_text_color(rating_table[i].get_color());
		name.set_pos(table_pos + b2Vec2(0, table_indent.y * i));

		kills.set_text(std::to_string(rating_table[i].get_kills()));
		kills.set_text_color(rating_table[i].get_color());
		kills.set_pos(table_pos + b2Vec2(table_name_indent, table_indent.y * i));

		deaths.set_text(std::to_string(rating_table[i].get_deaths()));
		deaths.set_text_color(rating_table[i].get_color());
		deaths.set_pos(table_pos + b2Vec2(table_name_indent + table_indent.x, table_indent.y * i));

		name.step();
		kills.step();
		deaths.step();
	}
}

void HUDProcessing::apply_bar(Bar* bar, std::stringstream* config) {
	int use_window_cords, use_custom_color, character_size;
	b2Vec2 pos, scale;
	sf::Color back, front, text;
	std::string font_name;
	*config >> use_window_cords >> pos.x >> pos.y >> scale.x >> scale.y >> character_size >> use_custom_color >> font_name;
	bar->set_use_window_cords(use_window_cords);
	bar->set_pos(pos);
	bar->set_scale(scale);
	bar->set_character_size(character_size);
	if (use_custom_color) {
		*config >> back.r >> back.g >> back.b >> back.a
			>> front.r >> front.g >> front.b >> front.a
			>> text.r >> text.g >> text.b >> text.a;
		bar->set_back_color(back);
		bar->set_front_color(front);
		bar->set_text_color(text);
	}
	bar->set_draw(draw);
	bar->set_mouse_pos(mouse_pos);
	bar->set_keyboard(keyboard);
	bar->set_font(font_name);
}

HUDProcessing::HUDProcessing()
{
}

HUDProcessing::HUDProcessing(Draw* draw_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_,
	GameClient* game_, ClientNetwork* player_network_, std::queue<int>* frame_marks_) {
	frame_marks = frame_marks_;
	game = game_;
	player_network = player_network_;
	std::ifstream file_to_comment("HUD.conf");
	std::stringstream config = aux::comment(file_to_comment);
	stamina_bar.set_front_color(sf::Color::Yellow);
	energy_bar.set_front_color(sf::Color::Cyan);
	draw = draw_;
	mouse_pos = mouse_pos_;
	keyboard = keyboard_;
	apply_bar(&HP_bar, &config);
	apply_bar(&stamina_bar, &config);
	apply_bar(&energy_bar, &config);
	config >> table_use_windows_cords >> table_pos.x >> table_pos.y >> table_character_size >>
		table_name_indent >> table_indent.x >> table_indent.y;

	ping.set_draw(draw);
	ping.set_mouse_pos(mouse_pos);
	ping.set_keyboard(keyboard);
	ping.set_use_window_cords(1);
	ping.set_text_character_pixel_size(17);
	ping.set_align(2);
	ping.set_pos({ 190, 50 });
	ping.set_text_scale(1);

	ping_text.set_draw(draw);
	ping_text.set_mouse_pos(mouse_pos);
	ping_text.set_keyboard(keyboard);
	ping_text.set_use_window_cords(1);
	ping_text.set_text_character_pixel_size(60);
	ping_text.set_text("Ping:");
	ping_text.set_pos({ 100, 50 });
	ping_text.set_text_scale(0.3);

	fps.set_draw(draw);
	fps.set_mouse_pos(mouse_pos);
	fps.set_keyboard(keyboard);
	fps.set_use_window_cords(1);
	fps.set_text_character_pixel_size(17);
	fps.set_align(2);
	fps.set_pos({ 190, 75 });
	fps.set_text_scale(1);

	fps_text.set_draw(draw);
	fps_text.set_mouse_pos(mouse_pos);
	fps_text.set_keyboard(keyboard);
	fps_text.set_use_window_cords(1);
	fps_text.set_text_character_pixel_size(60);
	fps_text.set_text("FPS:");
	fps_text.set_pos({ 100, 73 });
	fps_text.set_text_scale(0.3);

	time_to_respawn.set_draw(draw);
	time_to_respawn.set_mouse_pos(mouse_pos);
	time_to_respawn.set_keyboard(keyboard);
	time_to_respawn.set_use_window_cords(0);
	time_to_respawn.set_text_character_pixel_size(60);
	time_to_respawn.set_pos(b2Vec2(0, 0));
	time_to_respawn.set_text_scale(0.5);

	press_r_to_respawn.set_draw(draw);
	press_r_to_respawn.set_mouse_pos(mouse_pos);
	press_r_to_respawn.set_keyboard(keyboard);
	press_r_to_respawn.set_use_window_cords(0);
	press_r_to_respawn.set_text_character_pixel_size(60);
	press_r_to_respawn.set_pos(b2Vec2(0, 0));
	press_r_to_respawn.set_text_scale(0.5);

	bonus.set_draw(draw);
	bonus.set_use_window_cords(4);
	bonus.set_use_picture_scale(0);
	bonus.set_scale({ 100, 100 });
	bonus.set_color(sf::Color::White);
	bonus.set_pos({ -480, -105 });
	if (game->get_ship(player_network->get_id()) != nullptr)
		bonus.set_texture_name(game->get_bonus_texture_name(game->get_ship(player_network->get_id())->get_bonus_slot()->get_current_bonus()));
	else
		bonus.set_texture_name("bonusEmpty");

	bonus_tip.set_draw(draw);
	bonus_tip.set_use_window_cords(4);
	bonus_tip.set_text_character_pixel_size(100);
	bonus_tip.set_text_color(sf::Color(240, 240, 240, 255));
	bonus_tip.set_pos({ -480, -40 });

	gun_image.set_draw(draw);
	gun_image.set_use_window_cords(4);
	gun_image.set_use_picture_scale(0);
	gun_image.set_scale({ 78, 78 });
	gun_image.set_color(sf::Color::White);
	gun_image.set_pos({ -380, -105 });
	if (game->player_by_id(player_network->get_id()) != nullptr)
		gun_image.set_texture_name(game->player_by_id(player_network->get_id())->get_gun_name() + "-gun");
	else
		gun_image.set_texture_name("bonusEmpty");

	left_module.set_draw(draw);
	left_module.set_use_window_cords(4);
	left_module.set_use_picture_scale(0);
	left_module.set_scale({ 100, 100 });
	left_module.set_color(sf::Color::White);
	left_module.set_pos({ 380, -105});
	if (game->get_ship(player_network->get_id()) != nullptr)
		left_module.set_texture_name(Module::get_name_by_type(game->get_ship(player_network->get_id())->get_left_module()->get_type()) + "-module");
	else
		left_module.set_texture_name("NONE-module");

	left_module_tip.set_draw(draw);
	left_module_tip.set_use_window_cords(4);
	left_module_tip.set_text_character_pixel_size(100);
	left_module_tip.set_text_color(sf::Color(240, 240, 240, 255));
	left_module_tip.set_pos({ 380, -40 });

	right_module.set_draw(draw);
	right_module.set_use_window_cords(4);
	right_module.set_use_picture_scale(0);
	right_module.set_scale({ 100, 100 });
	right_module.set_color(sf::Color::White);
	right_module.set_pos({ 480, -105 });
	if (game->get_ship(player_network->get_id()) != nullptr)
		right_module.set_texture_name(Module::get_name_by_type(game->get_ship(player_network->get_id())->get_right_module()->get_type()) + "-module");
	else
		right_module.set_texture_name("NONE-module");

	right_module_tip.set_draw(draw);
	right_module_tip.set_use_window_cords(4);
	right_module_tip.set_text_character_pixel_size(100);
	right_module_tip.set_text_color(sf::Color(240, 240, 240, 255));
	right_module_tip.set_pos({ 480, -40 });

	HP_bar.set_value(HP_bar_val);
	HP_bar.set_max_value(HP_bar_max_val);
	stamina_bar.set_value(stamina_bar_val);
	stamina_bar.set_max_value(stamina_bar_max_val);
	energy_bar.set_value(energy_bar_val);
	energy_bar.set_max_value(energy_bar_max_val);

	interface_image.set_texture_name("interface");
	interface_image.set_color(sf::Color::White);
	interface_image.set_draw(draw);
	interface_image.set_use_window_cords(4);
	interface_image.set_pos({ 0, -20 });
	interface_image.set_use_picture_scale(0);
	interface_image.set_scale({ 330, 40 });
	//interface_image.set_scale({1220, 305});
}

void HUDProcessing::step() {
	get_buttons_names("keys.conf");
	interface_image.set_scale({ 330 * interface_scale, 40 * interface_scale });
	interface_image.set_pos(b2Vec2(0, 0) - b2Vec2(0, interface_image.get_scale().y / 2));
	b2Vec2 cur_icon_scale = interface_scale * b2Vec2(30, 30);
	bonus.set_scale(cur_icon_scale);
	bonus.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2) + interface_scale * b2Vec2(-130, 0));
	gun_image.set_scale(cur_icon_scale);
	gun_image.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2) + interface_scale * b2Vec2(-100, 0));
	right_module.set_scale(cur_icon_scale);
	right_module.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2) + interface_scale * b2Vec2(130, 0));
	left_module.set_scale(cur_icon_scale);
	left_module.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2) + interface_scale * b2Vec2(100, 0));
	HP_bar.set_scale(interface_scale * b2Vec2(130, 8.666666));
	stamina_bar.set_scale(interface_scale * b2Vec2(130, 8.666666));
	energy_bar.set_scale(interface_scale * b2Vec2(130, 8.666666));
	HP_bar.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2) + interface_scale * b2Vec2(0, -8.666666));
	stamina_bar.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2));
	energy_bar.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2) + interface_scale * b2Vec2(0, 8.666666));
	bonus_tip.set_text_scale(0.1 * interface_scale / text_mod);
	left_module_tip.set_text_scale(0.1 * interface_scale / text_mod);
	right_module_tip.set_text_scale(0.1 * interface_scale / text_mod);
	text_mod = 0.1 * interface_scale;
	bonus_tip.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2) + interface_scale * b2Vec2(-130, 0) + 0.3 * b2Vec2(cur_icon_scale.x, -cur_icon_scale.y));
	left_module_tip.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2) + interface_scale * b2Vec2(100, 0) + 0.3 * b2Vec2(cur_icon_scale.x, -cur_icon_scale.y));
	right_module_tip.set_pos(b2Vec2(0, -interface_image.get_scale().y / 2) + interface_scale * b2Vec2(130, 0) + 0.3 * b2Vec2(cur_icon_scale.x, -cur_icon_scale.y));
	if (game->get_ship(player_network->get_id()) != nullptr) {
		HP_bar.set_value(game->get_ship(player_network->get_id())->get_hp()->get());
		HP_bar.set_max_value(game->get_ship(player_network->get_id())->get_hp()->get_max());
		stamina_bar.set_value(game->get_ship(player_network->get_id())->get_stamina()->get());
		stamina_bar.set_max_value(game->get_ship(player_network->get_id())->get_stamina()->get_max());
		energy_bar.set_value(game->get_ship(player_network->get_id())->get_energy()->get());
		energy_bar.set_max_value(game->get_ship(player_network->get_id())->get_energy()->get_max());

		bonus.set_texture_name(game->get_bonus_texture_name(game->get_ship(player_network->get_id())->get_bonus_slot()->get_current_bonus()));
		left_module.set_texture_name(Module::get_name_by_type(game->get_ship(player_network->get_id())->get_left_module()->get_type()) + "-module");
		right_module.set_texture_name(Module::get_name_by_type(game->get_ship(player_network->get_id())->get_right_module()->get_type()) + "-module");
		//gun_image.set_texture_name(game->get_ship(player_network->get_id())->get_gun()->get_id());
	}
	if (game->player_by_id(player_network->get_id()) != nullptr &&
		!game->player_by_id(player_network->get_id())->get_is_alive()) {
		time_to_respawn.set_text("Time to respawn: " +
			std::to_string(int(game->player_by_id(player_network->get_id())->get_time_to_respawn()->get())));
		float mod = abs(sin(float(aux::get_milli_count()) / 500));
		press_r_to_respawn.set_text_color(sf::Color(255, 255, 255, 255 - 254 * mod));
		if (game->player_by_id(player_network->get_id())->get_time_to_respawn()->get() > 0)
			time_to_respawn.step();
		else
			press_r_to_respawn.step();
	}
	else {
		if (game->player_by_id(player_network->get_id()) != nullptr) {
			gun_image.set_texture_name(game->player_by_id(player_network->get_id())->get_gun_name() + "-gun");
			ping.set_text(std::to_string(game->player_by_id(player_network->get_id())->get_ping()) + "ms");
			//std::cout << ping.get_text() << "\n";
		}
		while (!frame_marks->empty() && frame_marks->front() + 1000 < aux::get_milli_count())
			frame_marks->pop();
		fps.set_text(std::to_string(frame_marks->size()));
		if (game->get_network_information_active()) {
			fps_text.step();
			fps.step();
			ping_text.step();
			ping.step();
		}
		if (game->get_ship(player_network->get_id()) != nullptr) {
			HP_bar.step();
			stamina_bar.step();
			energy_bar.step();
			interface_image.primitive_step();
			bonus.primitive_step();
			bonus_tip.step();
			auto mtype = game->get_ship(player_network->get_id())->get_left_module()->get_type();
			auto stam_param = game->get_module_manager()->get_prototype(mtype)->stamina_cost;
			auto ener_param = game->get_module_manager()->get_prototype(mtype)->energy_cost;
			if (stam_param >
				game->get_ship(player_network->get_id())->get_stamina()->get() ||
				ener_param >
				game->get_ship(player_network->get_id())->get_energy()->get())
				left_module.set_color(sf::Color(150, 150, 150, 255));
			else
				left_module.set_color(sf::Color::White);
			left_module.primitive_step();
			left_module_tip.step();
			mtype = game->get_ship(player_network->get_id())->get_right_module()->get_type();
			stam_param = game->get_module_manager()->get_prototype(mtype)->stamina_cost;
			ener_param = game->get_module_manager()->get_prototype(mtype)->energy_cost;
			if (stam_param >
				game->get_ship(player_network->get_id())->get_stamina()->get() ||
				ener_param >
				game->get_ship(player_network->get_id())->get_energy()->get())
				right_module.set_color(sf::Color(150, 150, 150, 255));
			else
				right_module.set_color(sf::Color::White);
			right_module.primitive_step();
			right_module_tip.step();
			gun_image.primitive_step();
			if (game->get_ship(player_network->get_id())->get_left_module()->get_recharge_counter()->get() > b2_epsilon) {
				draw->fill_polygon(get_vertices(game->get_ship(player_network->get_id())->get_left_module()->get_recharge_counter()->get() /
					game->get_ship(player_network->get_id())->get_left_module()->get_recharge_counter()->get_max(),
					left_module.window_cords_pos(), left_module.get_screen_mode() * left_module.get_scale()), sf::Color(0, 0, 0, 150));
			}
			if (game->get_ship(player_network->get_id())->get_right_module()->get_recharge_counter()->get() > b2_epsilon) {
				draw->fill_polygon(get_vertices(game->get_ship(player_network->get_id())->get_right_module()->get_recharge_counter()->get() /
					game->get_ship(player_network->get_id())->get_right_module()->get_recharge_counter()->get_max(),
					right_module.window_cords_pos(), right_module.get_screen_mode() * right_module.get_scale()), sf::Color(0, 0, 0, 150));
			}
		}
	}
	table_step();
	draw->draw_animations(GameClient::HUD);
}
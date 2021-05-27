#include "HUD_Processing.h"

bool operator<(Player& a, Player& b) {
	return (a.get_kills() > b.get_kills() ||
		(a.get_kills() == b.get_kills() && a.get_deaths() < b.get_deaths()));
}

std::string HUD_Processing::get_respawn_button_name(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	for (int i = 0; !config.eof(); i++) {
		std::string cur, cur_name;
		config >> cur_name >> cur;
		if (cur_name == "RESPAWN") {
			return cur;
		}
		for (int j = 0; !config.eof() && (cur != "END"); j++) {
			config >> cur;
		}
	}
}

std::vector<b2Vec2> HUD_Processing::get_vertices(float cur_pos, b2Vec2 center, b2Vec2 scale) {
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

void HUD_Processing::table_step() {
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

void HUD_Processing::apply_bar(Bar* bar, std::stringstream* config) {
	int use_window_cords, use_custom_color, character_size;
	b2Vec2 pos, scale;
	sf::Color back, front, text;
	*config >> use_window_cords >> pos.x >> pos.y >> scale.x >> scale.y >> character_size >> use_custom_color;
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
}

HUD_Processing::HUD_Processing(Draw* draw_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_,
	Game_Client* game_, Client_Network* player_network_, std::queue<int>* frame_marks_) {
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
	time_to_respawn.set_text_scale(0.5);

	press_r_to_respawn.set_draw(draw);
	press_r_to_respawn.set_mouse_pos(mouse_pos);
	press_r_to_respawn.set_keyboard(keyboard);
	press_r_to_respawn.set_use_window_cords(0);
	press_r_to_respawn.set_text_character_pixel_size(60);
	press_r_to_respawn.set_text_scale(0.5);

	bonus.set_draw(draw);
	bonus.set_use_window_cords(5);
	bonus.set_use_picture_scale(0);
	bonus.set_scale({ 150, 150 });
	bonus.set_color(sf::Color::White);
	bonus.set_pos({ -100, -100 });
	if (game->get_ship(player_network->get_id()) != nullptr)
		bonus.set_texture_name(game->get_bonus_texture_name(game->get_ship(player_network->get_id())->get_bonus_slot()->get_current_bonus()));
	else
		bonus.set_texture_name("bonusEmpty");

	left_module.set_draw(draw);
	left_module.set_use_window_cords(5);
	left_module.set_use_picture_scale(0);
	left_module.set_scale({ 150, 150 });
	left_module.set_color(sf::Color::White);
	left_module.set_pos({-310, -310});
	if (game->get_ship(player_network->get_id()) != nullptr)
		left_module.set_texture_name(Module::get_name_by_type(game->get_ship(player_network->get_id())->get_left_module()->get_type()) + "-module");
	else
		left_module.set_texture_name("NONE-module");

	right_module.set_draw(draw);
	right_module.set_use_window_cords(5);
	right_module.set_use_picture_scale(0);
	right_module.set_scale({ 150, 150 });
	right_module.set_color(sf::Color::White);
	right_module.set_pos({ -100, -310 });
	if (game->get_ship(player_network->get_id()) != nullptr)
		right_module.set_texture_name(Module::get_name_by_type(game->get_ship(player_network->get_id())->get_right_module()->get_type()) + "-module");
	else
		right_module.set_texture_name("NONE-module");

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
	interface_image.set_pos({ -10, -90 });
	interface_image.set_use_picture_scale(0);
	interface_image.set_scale({1200, 180});
}

void HUD_Processing::step() {
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
	}
	if (game->player_by_id(player_network->get_id()) != nullptr &&
		!game->player_by_id(player_network->get_id())->get_is_alive()) {
		time_to_respawn.set_text("Time to respawn: " +
			std::to_string(int(game->player_by_id(player_network->get_id())->get_time_to_respawn()->get())));
		float mod = abs(sin(float(aux::get_milli_count()) / 500));
		press_r_to_respawn.set_text_color(sf::Color(255, 255, 255, 255 - 254 * mod));
		press_r_to_respawn.set_text("Press " + get_respawn_button_name("keys.conf") + " to respawn");
		if (game->player_by_id(player_network->get_id())->get_time_to_respawn()->get() > 0)
			time_to_respawn.step();
		else
			press_r_to_respawn.step();
	}
	else {
		if (game->player_by_id(player_network->get_id()) != nullptr) {
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
		HP_bar.step();
		stamina_bar.step();
		energy_bar.step();
		interface_image.primitive_step();
		bonus.primitive_step();
		left_module.primitive_step();
		right_module.primitive_step();
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
	table_step();
	draw->draw_animations(Game_Client::HUD);
}
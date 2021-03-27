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

HUD_Processing::HUD_Processing(Draw* draw_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_, Game_Client* game_, Client_Network* player_network_) {
	game = game_;
	player_network = player_network_;
	std::ifstream file_to_comment("HUD.conf");
	std::stringstream config = aux::comment(file_to_comment);
	stamina_bar.set_front_color(sf::Color::Yellow);
	draw = draw_;
	mouse_pos = mouse_pos_;
	keyboard = keyboard_;
	apply_bar(&HP_bar, &config);
	apply_bar(&stamina_bar, &config);
	config >> table_use_windows_cords >> table_pos.x >> table_pos.y >> table_character_size >>
		table_name_indent >> table_indent.x >> table_indent.y;

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
}

void HUD_Processing::step() {
	if (game->get_ship(player_network->get_id()) != nullptr) {
		HP_bar.set_value(game->get_ship(player_network->get_id())->get_hp()->get());
		stamina_bar.set_value(game->get_ship(player_network->get_id())->get_stamina()->get());
	}
	if (!game->player_by_id(player_network->get_id())->get_is_alive()) {
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
	HP_bar.step();
	stamina_bar.step();
	table_step();
}
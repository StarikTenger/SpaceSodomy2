#include "HUD_Processing.h"

bool operator<(Player& a, Player& b) {
	return (a.get_kills() > b.get_kills() ||
		(a.get_kills() == b.get_kills() && a.get_deaths() < b.get_deaths()));
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
}

void HUD_Processing::step() {
	if (game->get_ship(player_network->get_id()) != nullptr) {
		HP_bar.set_value(game->get_ship(player_network->get_id())->get_hp()->get());
		stamina_bar.set_value(game->get_ship(player_network->get_id())->get_stamina()->get());
	}
	HP_bar.step();
	stamina_bar.step();
	table_step();
}
#include "HUD_Processing.h"

void HUD_Processing::apply_bar(Bar* bar, std::stringstream* config, Draw* draw, b2Vec2* mouse_pos, aux::Keyboard* keyboard) {
	int use_window_cords, use_custom_color, character_size;
	b2Vec2 pos, scale;
	sf::Color back, front, text;
	*config >> use_window_cords >> pos.x >> pos.y >> scale.x >> scale.y >> character_size >> use_custom_color;
	bar->set_use_window_cords(use_window_cords);
	bar->set_pos(pos);
	bar->set_scale(scale);
	if (use_custom_color) {
		*config >> back.r >> back.g >> back.b >> back.a
			>> front.r >> front.g >> front.b >> front.a
			>> text.r >> text.g >> text.b >> text.a;
		bar->back_color = back;
		bar->front_color = front;
		bar->text_color = text;
	}
	bar->set_draw(draw);
	bar->set_mouse_pos(mouse_pos);
	bar->set_keyboard(keyboard);
}

HUD_Processing::HUD_Processing(Draw* draw, b2Vec2* mouse_pos, aux::Keyboard* keyboard, Game_Client* game_, Client_Network* player_network_) {
	game = game_;
	player_network = player_network_;
	std::ifstream file_to_comment("HUD.conf");
	std::stringstream config = aux::comment(file_to_comment);
	apply_bar(&HP_bar, &config, draw, mouse_pos, keyboard);
}

void HUD_Processing::step() {
	HP_bar.value = game->get_ship(player_network->get_id())->get_hp()->get();
	HP_bar.step();
}
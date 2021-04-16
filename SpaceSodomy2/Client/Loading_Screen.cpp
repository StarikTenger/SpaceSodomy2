#include "Loading_Screen.h"

Loading_Screen::Loading_Screen(Draw* draw_) {
	draw = draw_;
	loading_stage.set_color(sf::Color::White);
	loading_stage.set_draw(draw);
	loading_stage.set_pos({ 0, 50 });
	loading_stage.set_text_character_pixel_size(30);
	progress_bar.set_back_color(sf::Color(140, 140, 140, 255));
	progress_bar.set_front_color(sf::Color(200, 200, 200, 255));
	progress_bar.set_scale({ 700, 50 });
	progress_bar.set_pos({ 0, 0 });
	progress_bar.set_character_size(0);
	progress_bar.set_draw(draw);
	progress_bar.set_max_value(100);
	progress_bar.set_value(0);
}

void Loading_Screen::step(int val, std::string stage) {
	draw->fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(draw->get_window()->getSize())),
		sf::Color(0, 0, 0, 255), 0);
	progress_bar.set_value(val);
	loading_stage.set_text(stage);
	draw->text("Loading...", "font", { 0, -50 }, 50, sf::Color::White);
	progress_bar.step();
	loading_stage.step();
	draw->display();
}

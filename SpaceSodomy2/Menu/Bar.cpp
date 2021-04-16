#include "pch.h"
#include "Bar.h"

Bar::Bar() {
	set_image_active(0);
	set_use_picture_scale(0);
}

// Set methods
void Bar::set_max_value(int max_value_) {
	max_value = max_value_;
	critical_value = 0.3 * max_value;
}
void Bar::set_value(float value_) {
	value = value_;
}
void Bar::set_character_size(unsigned int character_size_) {
	character_size = character_size_;
}
void Bar::set_back_color(sf::Color back_color_) {
	back_color = back_color_;
}
void Bar::set_front_color(sf::Color front_color_) {
	front_color = front_color_;
}
void Bar::set_text_color(sf::Color text_color_) {
	text_color = text_color_;
}
void Bar::set_angle(float angle_) {
	text.set_text_angle(angle_);
	angle = angle_;
}
void Bar::set_critical_value(float critical_value_) {
	critical_value = critical_value_;
}

// Get methods
int Bar::get_max_value() {
	return max_value;
}
float Bar::get_value() {
	return value;
}
unsigned int Bar::get_character_size() {
	return character_size;
}
sf::Color Bar::get_back_color() {
	return back_color;
}
sf::Color Bar::get_front_color() {
	return front_color;
}
sf::Color Bar::get_text_color() {
	return text_color;
}

bool Bar::is_critical() {
	return value < critical_value;
}

void Bar::step() {
	if (draw_text) {
		text.set_pos(1.0 / get_screen_mode() *get_pos());
		text.set_text(std::to_string(int(value)));
		text.set_color(text_color);
		text.set_text_character_pixel_size(character_size);
		text.set_draw(get_draw());
		text.set_mouse_pos(get_mouse_pos());
	}

	if (is_critical()) {
		float mod = (0.5 + sin(float(aux::get_milli_count()) / 100) / 2) * 0.15;
		current_back_color = sf::Color(
			back_color.r + (front_color.r - back_color.r) * mod,
			back_color.g + (front_color.g - back_color.g) * mod,
			back_color.b + (front_color.b - back_color.b) * mod,
			back_color.a + (front_color.a - back_color.a) * mod);
	}
	else
		current_back_color = back_color;

	primitive_step();
	//std::cout << get_pos().x << " " << get_pos().y << " " << get_scale().x << " " << get_scale().y << "\n";
	get_draw()->fill_rect(get_pos(), get_screen_mode() * get_scale(), current_back_color, angle);
	get_draw()->fill_rect(get_pos() - get_screen_mode() * b2Vec2(get_scale().x * (max_value - value) / max_value / 2, 0),
		get_screen_mode() * (get_scale() - b2Vec2(get_scale().x * (max_value - value) / max_value, 0)), front_color, angle);
	if (draw_text) 
		text.step();
}
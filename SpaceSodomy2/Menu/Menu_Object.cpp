#include "pch.h"
#include "Menu_Object.h"

Menu_Object::Menu_Object() {}
Menu_Object::Menu_Object(std::string texture_name_, Draw* draw_, b2Vec2 pos_, b2Vec2 scale_) {
	texture_name = texture_name_;
	draw = draw_;
	pos = pos_;
	scale = scale_;
}

// Get methods
std::string Menu_Object::get_texture_name() {
	return texture_name;
}
Draw* Menu_Object::get_draw() {
	return draw;
}
b2Vec2 Menu_Object::get_pos() {
	return pos;
}
b2Vec2 Menu_Object::get_scale() {
	return scale;
}
bool Menu_Object::get_active() {
	return active;
}

// Set methods
void Menu_Object::set_texture_name(std::string texture_name_) {
	texture_name = texture_name_;
}
void Menu_Object::set_draw(Draw* draw_) {
	draw = draw_;
}
void Menu_Object::set_pos(b2Vec2 pos_) {
	pos = pos_;
}
void Menu_Object::set_scale(b2Vec2 scale_) {
	scale = scale_;
}

void Menu_Object::primitive_step()
{
	if (sf::FloatRect(aux::to_vector2f(pos),
		aux::to_vector2f(scale)).contains(sf::Vector2f(sf::Mouse::getPosition())))
		active = 1;
	else
		active = 0;
	draw->image(texture_name, pos, scale, (0.0F), color);
}
#include "pch.h"
#include "Menu_Object.h"

Menu_Object::Menu_Object() {}
Menu_Object::Menu_Object(sf::Texture* texture_, sf::Sprite* sprite_, sf::Window* window_, int pos_x_, int pos_y_, int scale_x_, int scale_y_) {
	texture = texture_;
	sprite = sprite_;
	window = window_;
	pos_x = pos_x_;
	pos_y = pos_y_;
	scale_x = scale_x_;
	scale_y = scale_y_;
}

//Get methods
sf::Texture* Menu_Object::get_texture() {
	return texture;
}
sf::Sprite* Menu_Object::get_sprite() {
	return sprite;
}
sf::Window* Menu_Object::get_window() {
	return window;
}
int Menu_Object::get_pos_x() {
	return pos_x;
}
int Menu_Object::get_pos_y() {
	return pos_y;
}
int Menu_Object::get_scale_x() {
	return scale_x;
}
int Menu_Object::get_scale_y() {
	return scale_y;
}
bool Menu_Object::get_active() {
	return active;
}

//Set methods
void Menu_Object::set_texture(sf::Texture* texture_) {
	texture = texture_;
}
void Menu_Object::set_sprite(sf::Sprite* sprite_) {
	sprite = sprite_;
}
void Menu_Object::set_window(sf::Window* window_) {
	window = window_;
}
void Menu_Object::set_pos_x(int pos_x_) {
	pos_x = pos_x_;
}
void Menu_Object::set_pos_y(int pos_y_) {
	pos_y = pos_y_;
}
void Menu_Object::set_scale_x(int scale_x_) {
	scale_x = scale_x_;
}
void Menu_Object::set_scale_y(int scale_y_) {
	scale_y = scale_y_;
}

void Menu_Object::step()
{
	if (sf::IntRect(pos_x, pos_y, scale_x, scale_y).contains(sf::Mouse::getPosition(*window)))
		active = 1;
	else
		active = 0;
}
#include "pch.h"
#include "Menu_Object.h"
#define PI 3.14159265358979323846

Menu_Object::Menu_Object() {}
Menu_Object::Menu_Object(int id_, std::string texture_name_, Draw* draw_, b2Vec2 pos_, b2Vec2 scale_, sf::Color color_) {
	id = id_;
	texture_name = texture_name_;
	draw = draw_;
	pos = pos_;
	scale = scale_;
	color = color_;
}

// Get methods
int Menu_Object::get_id() {
	return id;
}
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
sf::Color Menu_Object::get_color() {
	return color;
}
bool Menu_Object::get_active() {
	return active;
}

// Set methods
void Menu_Object::set_id(int id_) {
	id = id_;
}
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
void Menu_Object::set_color(sf::Color color_) {
	color = color_;
}

void Menu_Object::primitive_step()
{
	sf::Vector2i rect_pos = sf::Vector2i(aux::to_vector2f(pos)), window_size = sf::Vector2i(draw->get_window()->getSize());
	sf::Vector2i window_pos = draw->get_window()->getPosition(), rect_scale;
	rect_scale.x = 300;
	rect_scale.y = 50;
	rect_pos.x *= 10;
	rect_pos.y *= 10;
	rect_pos.x += window_size.x / 2 + window_pos.x;
	rect_pos.y += window_size.y / 2 + window_pos.y;
	std::cout << "Mouse: " << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << " ";
	std::cout << "Rect: " << rect_pos.x << " " << rect_pos.y << " ";
	std::cout << "Window: " << window_size.x << " " << window_size.y << "\n";
	if (sf::IntRect(rect_pos, rect_scale).contains(sf::Mouse::getPosition()))
		active = 1;
	else
		active = 0;
	auto camera_settings = draw->get_camera();
	b2Vec2 camera_pos = camera_settings->get_pos();
	float camera_scale = camera_settings->get_scale();
	float camera_angle = camera_settings->get_angle();
	draw->apply_camera(b2Vec2(0, 0), 100, 0.0);
	draw->image(texture_name, pos, scale, 0.5 * PI, color);
	draw->apply_camera(camera_pos, camera_scale, camera_angle);
}
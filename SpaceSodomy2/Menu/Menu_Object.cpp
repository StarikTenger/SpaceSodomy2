#include "pch.h"
#include "Menu_Object.h"
#define PI 3.14159265358979323846

Menu_Object::Menu_Object() {}
Menu_Object::Menu_Object(int id_, std::string texture_name_, Draw* draw_, b2Vec2 pos_, b2Vec2 scale_, sf::Color color_, b2Vec2* mouse_pos_) {
	id = id_;
	texture_name = texture_name_;
	draw = draw_;
	pos = pos_;
	scale = scale_;
	color = color_;
	mouse_pos = mouse_pos_;
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
bool Menu_Object::get_use_picture_scale() {
	return use_picture_scale;
}
b2Vec2 Menu_Object::get_scale() {
	return scale;
}
sf::Color Menu_Object::get_color() {
	return color;
}
b2Vec2* Menu_Object::get_mouse_pos() {
	return mouse_pos;
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
void Menu_Object::set_use_picture_scale(bool use_picture_scale_) {
	use_picture_scale = use_picture_scale_;
}
void Menu_Object::set_scale(b2Vec2 scale_) {
	scale = scale_;
}
void Menu_Object::set_color(sf::Color color_) {
	color = color_;
}
void Menu_Object::set_mouse_pos(b2Vec2* mouse_pos_) {
	mouse_pos = mouse_pos_;
}

void Menu_Object::primitive_step()
{
	b2Vec2 mid = aux::to_b2vec2(sf::Vector2f(draw->get_window()->getSize()));
	mid.x /= 2;
	mid.y /= 2;
	b2Vec2 rect_pos = pos;
	if (use_picture_scale)
		scale = aux::to_b2vec2(sf::Vector2f(draw->get_texture(texture_name)->getSize()));
	//std::cout << "Mouse: " << mouse_pos->x << " " << mouse_pos->y << " " 
	//	<< aux::rect_contains(rect_pos + mid, scale, *mouse_pos) << "\n";
	if (aux::rect_contains(rect_pos + mid, scale, *mouse_pos))
		active = 1;
	else
		active = 0;
	auto camera_settings = draw->get_camera();
	b2Vec2 camera_pos = camera_settings->get_pos();
	float camera_scale = camera_settings->get_scale();
	float camera_angle = camera_settings->get_angle();
	draw->apply_camera(b2Vec2(0, 0), 1, 1.5*PI);
	draw->image(texture_name, pos, scale, 0, color);
	draw->apply_camera(camera_pos, camera_scale, camera_angle);
}
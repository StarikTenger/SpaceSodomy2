#include "pch.h"
#include "Menu_Object.h"

Menu_Object::Menu_Object() {}
Menu_Object::Menu_Object(int id_, std::string texture_name_, Draw* draw_,
	b2Vec2 pos_, b2Vec2 scale_, sf::Color color_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_) {
	id = id_;
	texture_name = texture_name_;
	draw = draw_;
	pos = pos_;
	scale = scale_;
	color = color_;
	mouse_pos = mouse_pos_;
	keyboard = keyboard_;
}

b2Vec2 Menu_Object::window_cords_pos() {
	mid = aux::to_b2Vec2(sf::Vector2f(draw->get_window()->getSize())); // Mid screen indent
	mid.x /= 2;
	mid.y /= 2;
	switch (use_window_cords)
	{
	case 0:
		return (screen_mode * pos);
	case 1:
		return (screen_mode * pos) - mid;
	case 2:
		return (screen_mode * pos) - b2Vec2(mid.x, 0);
	case 3:
		return (screen_mode * pos) + b2Vec2(-mid.x, mid.y);
	case 4:
		return (screen_mode * pos) + b2Vec2(0, mid.y);
	case 5:
		return (screen_mode * pos) + mid;
	case 6:
		return (screen_mode * pos) + b2Vec2(mid.x, 0);
	case 7:
		return (screen_mode * pos) + b2Vec2(mid.x, -mid.y);
	case 8:
		return (screen_mode * pos) - b2Vec2(0, mid.y);
	default:
		return (screen_mode * pos);
	}
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
	return window_cords_pos();
}
bool Menu_Object::get_use_picture_scale() {
	return use_picture_scale;
}
bool Menu_Object::get_image_active() {
	return image_active;
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
aux::Keyboard* Menu_Object::get_keyboard() {
	return keyboard;
}
bool Menu_Object::get_active() {
	return active;
}
bool* Menu_Object::get_clicked() {
	return clicked;
}
int Menu_Object::get_use_window_cords() {
	return use_window_cords;
}
b2Vec2 Menu_Object::get_cur_pos() {
	return cur_pos;
}
double Menu_Object::get_screen_mode() {
	return screen_mode;
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
void Menu_Object::set_image_active(bool image_active_) {
	image_active = image_active_;
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
void Menu_Object::set_keyboard(aux::Keyboard* keyboard_) {
	keyboard = keyboard_;
}
void Menu_Object::set_clicked(bool* clicked_) {
	clicked = clicked_;
}
void Menu_Object::set_use_window_cords(int use_window_cords_) {
	use_window_cords = use_window_cords_;
}
void Menu_Object::set_cur_pos(b2Vec2 cur_pos_) {
	cur_pos = cur_pos_;
	cur_pos_activated = 1;
}

void Menu_Object::primitive_step() {
	if (image_active && use_picture_scale)
		scale = aux::to_b2Vec2(sf::Vector2f(draw->get_texture(texture_name)->getSize()));
	screen_mode = ((double)draw->get_window()->getSize().x / sf::VideoMode::getDesktopMode().width +
		(double)draw->get_window()->getSize().y / sf::VideoMode::getDesktopMode().height) / 2;
	b2Vec2 backup_scale = scale;
	scale = screen_mode * scale;
	if (!cur_pos_activated) {
		cur_pos = window_cords_pos();
	}
	//std::cout << texture_name << " " << scale.x << " " << scale.y << "\n";
	if (aux::rect_contains(cur_pos + mid, scale, *mouse_pos))
		active = 1;
	else
		active = 0;
	if (image_active)
		draw->image(texture_name, cur_pos, scale, 0, color);
	scale = backup_scale;
}
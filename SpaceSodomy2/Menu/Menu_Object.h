#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <Draw/Draw.h>
#include <AuxLib/AuxLib.h>

class Menu_Object {
private:
	// Object global menu id
	int id;
	std::string texture_name;
	sf::Color color;
	Draw* draw;
	b2Vec2 pos, scale;
	// Is object active (mouse pointed on it)
	bool active = 0;
	b2Vec2* mouse_pos;
	aux::Keyboard* keyboard;
	// Use image scale in pixels
	bool use_picture_scale = 1;
	// Rendering active
	bool image_active = 1;
	// Mouse clicked
	bool* clicked = 0;
	int use_window_cords = 0;
public:
	Menu_Object();
	Menu_Object(int id_, std::string texture_name_, Draw* draw_, 
		b2Vec2 pos_, b2Vec2 scale_, sf::Color color_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_);

	// Get methods
	int get_id();
	std::string get_texture_name();
	Draw* get_draw();
	b2Vec2 get_pos();
	bool get_use_picture_scale();
	bool get_image_active();
	b2Vec2 get_scale();
	sf::Color get_color();
	b2Vec2* get_mouse_pos();
	aux::Keyboard* get_keyboard();
	bool get_active();
	bool* get_clicked();
	int get_use_window_cords();

	// Set methods
	void set_id(int id_);
	void set_texture_name(std::string texture_name_);
	void set_draw(Draw* draw_);
	void set_pos(b2Vec2 pos_);
	void set_use_picture_scale(bool use_picture_scale_);
	void set_image_active(bool image_active_);
	void set_scale(b2Vec2 scale_);
	void set_color(sf::Color color_);
	void set_mouse_pos(b2Vec2* mouse_pos_);
	void set_keyboard(aux::Keyboard* keyboard);
	void set_clicked(bool* clicked_);
	void set_use_window_cords(int use_window_cords_);

	void primitive_step(); // Rendering & active check
};


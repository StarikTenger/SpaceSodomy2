#pragma once
#include "Menu_Object.h"
class Keyboard_Field : public Menu_Object {
private:
	sf::Text text;
	b2Vec2 indent = { 5, 5 }; // indent above text size
	bool keyboard_active = 0;
	bool font_setted = 0;
	float height;
	int align = 0;
public:
	Keyboard_Field();

	// Get methods
	std::string get_text();
	sf::Font get_font();
	sf::Color get_text_color();
	unsigned int get_text_character_pixels_size();
	b2Vec2 get_indent();
	bool get_keyboard_active();

	// Set methods
	void set_text(std::string text_);
	void set_font(sf::Font font_);
	void set_text_color(sf::Color);
	void set_text_character_pixel_size(unsigned int size_);
	void set_indent(b2Vec2 indent_);
	void set_keyboard_active(bool keyboard_active_);
	void set_align(int align);

	void step();
};


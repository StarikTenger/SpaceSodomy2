#pragma once
#include "Menu_Object.h"
class Text_Field : public Menu_Object
{
private:
	sf::Text text;
	b2Vec2 indent = { 5, 5 };
public:
	Text_Field();
	
	// Get methods
	std::string get_text();
	sf::Font get_font();
	sf::Color get_text_color();
	unsigned int get_text_character_pixels_size();
	b2Vec2 get_indent();


	// Set methods
	void set_text(const wchar_t* text_);
	void set_font(sf::Font font_);
	void set_text_color(sf::Color);
	void set_text_character_pixel_size(unsigned int size_);
	void set_indent(b2Vec2 indent_);

	void step();
};


#pragma once
#include <math.h>
#include "Menu_Object.h"
#include "Constant_Text.h"

class Bar : public Menu_Object {
private:
	Constant_Text text;
	sf::Color current_back_color;
	float max_value = 100;
	float value = 50;
	float critical_value = 30;
	float angle = 0;
	unsigned int character_size = 10;
	sf::Color back_color = sf::Color(60, 60, 60, 190);
	sf::Color front_color = sf::Color::Red;
	sf::Color text_color = sf::Color::White;
public:
	Bar();

	bool draw_text = 1;

	// Set methods
	void set_max_value(float max_value_);
	void set_value(float value_);
	void set_character_size(unsigned int character_size_);
	void set_back_color(sf::Color back_color_);
	void set_front_color(sf::Color front_color_);
	void set_text_color(sf::Color text_color_);
	void set_angle(float angle_);

	// Get methods
	float get_max_value();
	float get_value();
	unsigned int get_character_size();
	sf::Color get_back_color();
	sf::Color get_front_color();
	sf::Color get_text_color();


	bool is_critical();

	void step();
};


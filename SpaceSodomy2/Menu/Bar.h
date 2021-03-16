#pragma once
#include "Menu_Object.h"
#include "Constant_Text.h"

class Bar : public Menu_Object {
private:
	Constant_Text text;
public:
	Bar();

	unsigned int character_size = 10;
	float max_value = 100;
	float value = 50;
	sf::Color back_color = sf::Color(60, 60, 60, 190);
	sf::Color front_color = sf::Color::Red;
	sf::Color text_color = sf::Color::White;

	void step();
};


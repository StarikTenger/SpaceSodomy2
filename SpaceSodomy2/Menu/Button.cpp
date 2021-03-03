#include "pch.h"
#include "Button.h"

Button::Button() {
	set_color(sf::Color::White);
}

void Button::step() {
	primitive_step();
	if (get_active())
		set_color(sf::Color::Cyan);
	else
		set_color(sf::Color::White);
}
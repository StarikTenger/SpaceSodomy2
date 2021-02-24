#pragma once
#include "Button.h"
#include <vector>

class Menu {
private:
	std::vector<Button*> buttons;
	sf::Window* window;
public:
	Menu();
	Menu(sf::Window* window_);

	void add_button(std::string texture_path, int pos_x, int pos_y, int scale_x, int scale_y);
	void step();
};
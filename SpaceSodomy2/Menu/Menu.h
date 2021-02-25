#pragma once
#include "Button.h"
#include <vector>

class Menu {
private:
	std::vector<Button*> buttons;
	sf::Window* window;
	bool active = 0;
public:
	Menu();
	Menu(sf::Window* window_);

	void set_active(bool active_);
	bool get_active();
	void add_button(int id, std::string texture_path, int pos_x, int pos_y, int scale_x, int scale_y);
	void step();
};
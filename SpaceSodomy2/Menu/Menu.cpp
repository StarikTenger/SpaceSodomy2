// Menu.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "Menu.h"
#include <Draw/Draw.h>

Menu::Menu() {}
Menu::Menu(sf::Window* window_) {
	window = window_;
}

void Menu::set_active(bool active_) {
	active = active_;
}

bool Menu::get_active() {
	return active;
}

void Menu::add_button(int id, std::string texture_path, int pos_x, int pos_y, int scale_x, int scale_y) {
	buttons.push_back(new Button);
	auto texture = new sf::Texture;
	texture->loadFromFile(texture_path);
	buttons.back()->set_texture(texture);
	auto sprite = new sf::Sprite(*texture);
	buttons.back()->set_sprite(sprite);
	buttons.back()->set_pos_x(pos_x);
	buttons.back()->set_pos_y(pos_y);
	buttons.back()->set_scale_x(scale_x);
	buttons.back()->set_scale_y(scale_y);
}

void Menu::step() {
	if (!active)
		return;
	bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	for (auto button : buttons) {
		button->step();
		if (pressed && button->get_active()) {}
		//TODO: button action
	}
}
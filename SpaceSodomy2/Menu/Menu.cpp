// Menu.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "Menu.h"
#include <Draw/Draw.h>

Menu::Menu() {}
Menu::Menu(Draw* draw_) {
	draw = draw_;
}

//Set_methods
void Menu::set_draw(Draw* draw_) {
	draw = draw_;
}

void Menu::set_active(bool active_) {
	active = active_;
}

void Menu::set_events(std::queue<int>* events_) {
	events = events_;
}

//Get_methods
Draw* Menu::get_draw() {
	return draw;
}

bool Menu::get_active() {
	return active;
}

std::queue<int>* Menu::get_events() {
	return events;
}

void Menu::add_button(int id, std::string texture_name, float pos_x, float pos_y, float scale_x, float scale_y, sf::Color color, b2Vec2* mouse_pos) {
	buttons.push_back(new Button);
	buttons.back()->set_id(id);
	buttons.back()->set_texture_name(texture_name);
	buttons.back()->set_pos(b2Vec2(pos_x, pos_y));
	buttons.back()->set_scale(b2Vec2(scale_x, scale_y));
	buttons.back()->set_color(color);
	buttons.back()->set_draw(draw);
	buttons.back()->set_mouse_pos(mouse_pos);
}

void Menu::step() {
	if (!active)
		return;
	bool pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	for (auto button : buttons) {
		button->step();
		if (pressed && button->get_active())
		{
			events->push(button->get_id());
			pressed = 0;
		}
	}
}
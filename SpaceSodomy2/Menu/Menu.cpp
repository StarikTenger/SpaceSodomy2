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
	buttons.back()->set_clicked(&clicked);
}

void Menu::add_text_field(int id, const wchar_t* text, std::string texture_name,float pos_x, float pos_y, float scale_x, float scale_y, sf::Color color, b2Vec2* mouse_pos, aux::Keyboard* keyboard) {
	text_fields.push_back(new Text_Field);
	text_fields.back()->set_id(id);
	text_fields.back()->set_text(text);
	text_fields.back()->set_texture_name(texture_name);
	text_fields.back()->set_pos(b2Vec2(pos_x, pos_y));
	text_fields.back()->set_scale(b2Vec2(scale_x, scale_y));
	text_fields.back()->set_color(color);
	text_fields.back()->set_draw(draw);
	text_fields.back()->set_mouse_pos(mouse_pos);
	text_fields.back()->set_keyboard(keyboard);
	text_fields.back()->set_clicked(&clicked);
}

void Menu::add_slider(int id, float pos_x, float pos_y, float axis_width, float axis_height, float slider_width, float slider_height, b2Vec2* mouse_pos) {
	sliders.push_back(new Slider);
	sliders.back()->set_id(id);
	sliders.back()->set_pos(b2Vec2(pos_x, pos_y));
	sliders.back()->set_draw(draw);
	sliders.back()->set_mouse_pos(mouse_pos);
	sliders.back()->setAxisWidth(axis_width);
	sliders.back()->setAxisHeight(axis_height);
	sliders.back()->setSliderWidth(slider_width);
	sliders.back()->setSliderHeight(slider_height);
	sliders.back()->set_clicked(&clicked);
	sliders.back()->create(0, 100);
	sliders.back()->init();
}

void Menu::step() {
	if (!active)
		return;
	clicked = (last_mouse_status == 0) && (sf::Mouse::isButtonPressed(sf::Mouse::Left));
	last_mouse_status = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	// Camera backup
	Camera camera_backup = *draw->get_camera();
	draw->apply_camera(b2Vec2(0, 0), 1, 1.5 * b2_pi);

	for (auto button : buttons) {
		button->step();
		if (clicked && button->get_active())
		{
			events->push(button->get_id());
			clicked = 0;
		}
	}
	for (auto text_field : text_fields) {
		text_field->step();
		if (clicked && !text_field->get_active() && text_field->get_keyboard_active())
			text_field->set_keyboard_active(0);
	}
	for (auto text_field : text_fields) {
		if (clicked && text_field->get_active())
		{
			while (!text_field->get_keyboard()->text_entered->empty())
				text_field->get_keyboard()->text_entered->pop();
			text_field->set_keyboard_active(1);
		}
	}
	for (auto slider : sliders) {
		slider->step();
	}

	// Restore camera
	draw->set_camera(camera_backup);
}
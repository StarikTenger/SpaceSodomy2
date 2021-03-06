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
void Menu::set_text_fields_strings(std::map<int, std::string>* text_fields_strings_) {
	text_fields_strings = text_fields_strings_;
}
void Menu::set_sliders_vals(std::map<int, int>* sliders_vals_) {
	sliders_vals = sliders_vals_;
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

void Menu::add_button(int id, std::string texture_name, b2Vec2 pos, int use_window_cords, b2Vec2 scale,
	sf::Color color, b2Vec2* mouse_pos) {
	buttons.push_back(new Button);
	buttons.back()->set_id(id);
	buttons.back()->set_texture_name(texture_name);
	buttons.back()->set_pos(pos);
	buttons.back()->set_use_window_cords(use_window_cords);
	buttons.back()->set_scale(scale);
	buttons.back()->set_color(color);
	buttons.back()->set_draw(draw);
	buttons.back()->set_mouse_pos(mouse_pos);
	buttons.back()->set_clicked(&clicked);
}

void Menu::add_text_field(int id, std::string text, std::string texture_name, b2Vec2 pos, int use_window_cords,
	int character_size, sf::Color color, b2Vec2* mouse_pos, aux::Keyboard* keyboard) {
	text_fields.push_back(new Text_Field);
	text_fields.back()->set_id(id);
	text_fields.back()->set_text(text);
	text_fields.back()->set_texture_name(texture_name);
	text_fields.back()->set_pos(pos);
	text_fields.back()->set_use_window_cords(use_window_cords);
	text_fields.back()->set_text_character_pixel_size(character_size);
	text_fields.back()->set_color(color);
	text_fields.back()->set_draw(draw);
	text_fields.back()->set_mouse_pos(mouse_pos);
	text_fields.back()->set_keyboard(keyboard);
	text_fields.back()->set_clicked(&clicked);
}

void Menu::add_keyboard_field(int id, std::string text, std::string texture_name, b2Vec2 pos, int use_window_cords,
	int character_size, sf::Color color, b2Vec2* mouse_pos, aux::Keyboard* keyboard) {
	keyboard_fields.push_back(new Keyboard_Field);
	keyboard_fields.back()->set_id(id);
	keyboard_fields.back()->set_text(text);
	keyboard_fields.back()->set_texture_name(texture_name);
	keyboard_fields.back()->set_pos(pos);
	keyboard_fields.back()->set_use_window_cords(use_window_cords);
	keyboard_fields.back()->set_text_character_pixel_size(character_size);
	keyboard_fields.back()->set_color(color);
	keyboard_fields.back()->set_draw(draw);
	keyboard_fields.back()->set_mouse_pos(mouse_pos);
	keyboard_fields.back()->set_keyboard(keyboard);
	keyboard_fields.back()->set_clicked(&clicked);
}

void Menu::add_constant_text(int id, std::string text, b2Vec2 pos, int use_window_cords,
	int character_size, sf::Color color, b2Vec2* mouse_pos, aux::Keyboard* keyboard) {
	constant_texts.push_back(new Constant_Text);
	constant_texts.back()->set_id(id);
	constant_texts.back()->set_text(text);
	constant_texts.back()->set_pos(pos);
	constant_texts.back()->set_use_window_cords(use_window_cords);
	constant_texts.back()->set_text_character_pixel_size(character_size);
	constant_texts.back()->set_color(color);
	constant_texts.back()->set_draw(draw);
	constant_texts.back()->set_mouse_pos(mouse_pos);
	constant_texts.back()->set_keyboard(keyboard);
	constant_texts.back()->set_clicked(&clicked);
}

void Menu::add_slider(int id, b2Vec2 pos, int use_window_cords, b2Vec2 axis_scale, b2Vec2 slider_scale,
	int min, int max, int val, b2Vec2* mouse_pos) {
	sliders.push_back(new Slider);
	sliders.back()->set_id(id);
	sliders.back()->set_pos(pos);
	sliders.back()->set_use_window_cords(use_window_cords);
	sliders.back()->set_draw(draw);
	sliders.back()->set_mouse_pos(mouse_pos);
	sliders.back()->set_axis_scale(axis_scale);
	sliders.back()->set_slider_scale(slider_scale);
	sliders.back()->set_clicked(&clicked);
	sliders.back()->create(min, max);
	sliders.back()->set_slider_value(val);
	sliders.back()->init();
}

void Menu::step() {
	if (!active)
		return;
	// set clicked val
	clicked = (last_mouse_status == 0) && (sf::Mouse::isButtonPressed(sf::Mouse::Left));
	last_mouse_status = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	// Camera backup
	Camera camera_backup = *draw->get_camera();
	draw->apply_camera(b2Vec2(0, 0), 1, 1.5 * b2_pi);

	// buttons handling
	for (auto button : buttons) {
		button->step();
		if (clicked && button->get_active()) {
			events->push(button->get_id());
			clicked = 0;
		}
	}

	// text fields handling
	for (auto text_field : text_fields) {
		text_field->step();
		text_fields_strings->operator[](text_field->get_id()) = text_field->get_text();
		if (clicked && !text_field->get_active() && text_field->get_keyboard_active())
			text_field->set_keyboard_active(0);
	}
	for (auto text_field : text_fields) {
		if (clicked && text_field->get_active()) {
			while (!text_field->get_keyboard()->text_entered->empty())
				text_field->get_keyboard()->text_entered->pop();
			text_field->set_keyboard_active(1);
		}
	}

	// keyboard fields handling
	for (auto keyboard_field : keyboard_fields) {
		keyboard_field->step();
		text_fields_strings->operator[](keyboard_field->get_id()) = keyboard_field->get_text();
		if (clicked && !keyboard_field->get_active() && keyboard_field->get_keyboard_active())
			keyboard_field->set_keyboard_active(0);
	}
	for (auto keyboard_field : keyboard_fields) {
		if (clicked && keyboard_field->get_active()) {
			keyboard_field->set_keyboard_active(1);
		}
	}

	// constant texts handling
	for (auto constant_text : constant_texts) {
		constant_text->step();
	}

	// sliders handling
	for (auto slider : sliders) {
		slider->step();
		sliders_vals->operator[](slider->get_id()) = slider->get_slider_value();
	}

	// Restore camera
	draw->set_camera(camera_backup);
}
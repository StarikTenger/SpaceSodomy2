#include "Menu_Processing.h"

Menu_Processing::Menu_Processing() {}

void Menu_Processing::init_menu(std::string path_, Menu* object) {
	// uncomment config file
	std::ifstream comment_file(path_);
	std::stringstream file = aux::comment(comment_file);
	for (int i = current_id; !file.eof(); i++) {
		current_id++;
		std::string type;
		int typenum = 0;
		std::string texture_name;
		b2Vec2 pos, scale;
		b2Vec2 axis_scale, slider_scale;
		int min_val, int max_val, int val;
		file >> type;
		if (type == "Button")
			typenum = 1;
		if (type == "TextField")
			typenum = 2;
		if (type == "Slider")
			typenum = 3;
		switch (typenum) {
		case 1:
			file >> texture_name;
			file >> pos.x >> pos.y >> scale.x >> scale.y;
			object->add_button(i, texture_name, pos, scale, sf::Color::White, mouse_pos);
			break;
		case 2:
			file >> texture_name;
			file >> pos.x >> pos.y >> scale.x >> scale.y;
			object->add_text_field(i, text_fields_strings[i], texture_name, pos, scale, sf::Color::White,
				mouse_pos, keyboard);
			break;
		case 3:
			file >> pos.x >> pos.y;
			file >> axis_scale.x >> axis_scale.y >> slider_scale.x >> slider_scale.y;
			file >> min_val >> max_val >> val;
			object->add_slider(i, pos, axis_scale, slider_scale,
				min_val, max_val, val, mouse_pos);
			break;
		default:
			break;
		}
	}
}

void Menu_Processing::init(Draw* draw_, b2Vec2* mouse_pos_,
	aux::Keyboard* keyboard_, void(*ptr)(std::string)) {
	draw = draw_;
	keyboard = keyboard_;
	mouse_pos = mouse_pos_;
	load_config_ptr = ptr;
	// set main menu fields
	main_menu.set_draw(draw);
	main_menu.set_active(1);
	main_menu.set_events(&events);
	main_menu.set_sliders_vals(&sliders_vals);
	main_menu.set_text_fields_strings(&text_fields_strings);
	// set client config menu fields
	config_menu.set_draw(draw);
	config_menu.set_active(0);
	config_menu.set_events(&events);
	config_menu.set_sliders_vals(&sliders_vals);
	config_menu.set_text_fields_strings(&text_fields_strings);
	// set keys menu fields
	keys_menu.set_draw(draw);
	keys_menu.set_active(0);
	keys_menu.set_events(&events);
	keys_menu.set_sliders_vals(&sliders_vals);
	keys_menu.set_text_fields_strings(&text_fields_strings);
	return;
}

void Menu_Processing::step() {
	main_menu.step();
	config_menu.step();
	keys_menu.step();
	while (!events.empty()){
		switch (events.front()) {
		case 1: // New game button
			main_menu.set_active(0);
			break;
		case 2: // About button
			std::cout << "well, it works";
			break;
		case 3: // Exit button
			draw->get_window()->close();
			break;
		default:
			break;
		}
		events.pop();
	}
}
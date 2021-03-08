#include "Menu_Processing.h"

Menu_Processing::Menu_Processing() {}

void Menu_Processing::save_config(std::string path, std::string address_, int port_, int id_, std::string name_) {
	std::ofstream fout;
	fout.open(path);
	fout << address_ << " " << port_ << " " << id_ << " " << name_;
	fout.close();
}
void Menu_Processing::load_config(std::string path, std::string* address_, std::string* port_,
	std::string* id_, std::string* name_) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	config >> *(address_);
	config >> *(port_);
	config >> *(id_);
	config >> *(port_);
	config >> *(name_);
}

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
		int min_val, max_val, val;
		int use_window_cords;
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
			file >> use_window_cords >> pos.x >> pos.y >> scale.x >> scale.y;
			object->add_button(i, texture_name, pos, use_window_cords, scale, sf::Color::White, mouse_pos);
			break;
		case 2:
			file >> texture_name;
			file >> use_window_cords >> pos.x >> pos.y >> scale.x >> scale.y;
			object->add_text_field(i, text_fields_strings[i], texture_name, pos, use_window_cords, scale, sf::Color::White,
				mouse_pos, keyboard);
			break;
		case 3:
			file >> use_window_cords >> pos.x >> pos.y;
			file >> axis_scale.x >> axis_scale.y >> slider_scale.x >> slider_scale.y;
			file >> min_val >> max_val >> val;
			object->add_slider(i, pos, use_window_cords, axis_scale, slider_scale,
				min_val, max_val, val, mouse_pos);
			break;
		default:
			i--;
			current_id--;
			break;
		}
	}
}

void Menu_Processing::init(Draw* draw_, b2Vec2* mouse_pos_,
	aux::Keyboard* keyboard_, bool* reload_) {
	draw = draw_;
	keyboard = keyboard_;
	mouse_pos = mouse_pos_;
	reload = reload_;
	// set main menu fields
	main_menu.set_draw(draw);
	main_menu.set_active(1);
	main_menu.set_events(&events);
	main_menu.set_sliders_vals(&sliders_vals);
	main_menu.set_text_fields_strings(&text_fields_strings);
	menus.push_back(&main_menu);
	init_menu("menu_configs/main.conf", &main_menu);
	// set client config menu fields
	config_menu.set_draw(draw);
	config_menu.set_active(0);
	config_menu.set_events(&events);
	config_menu.set_sliders_vals(&sliders_vals);
	config_menu.set_text_fields_strings(&text_fields_strings);
	load_config("client_config.conf", &text_fields_strings[5], &text_fields_strings[6],
		&text_fields_strings[7], &text_fields_strings[8]);
	menus.push_back(&config_menu);
	init_menu("menu_configs/client_config.conf", &config_menu);
	// set keys menu fields
	keys_menu.set_draw(draw);
	keys_menu.set_active(0);
	keys_menu.set_events(&events);
	keys_menu.set_sliders_vals(&sliders_vals);
	keys_menu.set_text_fields_strings(&text_fields_strings);
	menus.push_back(&keys_menu);
	init_menu("menu_configs/keys.conf", &keys_menu);
	// set settigs menu 
	settings_menu.set_draw(draw);
	settings_menu.set_active(0);
	settings_menu.set_events(&events);
	settings_menu.set_sliders_vals(&sliders_vals);
	settings_menu.set_text_fields_strings(&text_fields_strings);
	menus.push_back(&settings_menu);
	init_menu("menu_configs/settings.conf", &settings_menu);
}

void Menu_Processing::step() {
	for (auto menu : menus) {
		menu->step();
	}
	while (!events.empty()){
		switch (events.front()) {
		case 1: // New game button
			main_menu.set_active(0);
			break;
		case 2: // About button
			std::cout << "well, it works";
			break;
		case 3:
			main_menu.set_active(0);
			settings_menu.set_active(1);
			config_menu.set_active(1);
			break;
		case 4: // Exit button
			draw->get_window()->close();
			break;
		case 9: // Back button
			main_menu.set_active(1);
			settings_menu.set_active(0);
			config_menu.set_active(0);
			keys_menu.set_active(0);
			break;
		case 10: // Main button
			config_menu.set_active(1);
			keys_menu.set_active(0);
			break;
		case 11: // Control button
			config_menu.set_active(0);
			keys_menu.set_active(1);
			break;
		default:
			break;
		}
		events.pop();
	}
}
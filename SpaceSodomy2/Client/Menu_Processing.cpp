#include "Menu_Processing.h"

Menu_Processing::Menu_Processing() {}

void Menu_Processing::save_keys(std::string path, std::vector<std::vector<std::string*>> keys) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	std::vector<std::string> names;
	while (!config.eof()) {
		std::string cur;
		config >> cur;
		names.push_back(cur);
		while (!config.eof() && (cur != "END")) {
			config >> cur;
		}
	}
	if (names.back() == "")
		names.pop_back();
	std::ofstream fout;
	fout.open(path);
	for (int i = 0; i < names.size(); i++) {
		fout << names[i] << " ";
		for (int j = 0; j < keys[i].size(); j++)
			fout << *keys[i][j] << " ";
		fout << "END\n";
	}
	fout.close();
}
void Menu_Processing::load_keys(std::string path, std::vector<std::vector<std::string*>>* keys, Menu* menu,
	b2Vec2 pos, float name_indent, b2Vec2 indent, int character_size) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	for (int i = 0; !config.eof(); i++) {
		if (keys->size() == i)
			keys->push_back(std::vector<std::string*>());
		std::string cur, cur_name;
		config >> cur_name >> cur;
		for (int j = 0; !config.eof() && (cur != "END"); j++) {
			if (j == 0) {
				menu->add_constant_text(current_id, cur_name,
					b2Vec2(pos.x - name_indent, pos.y + indent.y * i), 0, character_size, sf::Color::White,
					2, mouse_pos, keyboard);
				current_id++;
			}
			if (j == keys->operator[](i).size()) {
				keys->operator[](i).push_back(&text_fields_strings[current_id]);
				text_fields_strings[current_id] = cur;
				menu->add_keyboard_field(current_id, text_fields_strings[current_id], "TextField",
					b2Vec2(pos.x + indent.x * (j + 1), pos.y + indent.y * i), 0, character_size, sf::Color::White,
					1, mouse_pos, keyboard);
				current_id++;
			}
			else
				*(keys->operator[](i)[j]) = cur;
			config >> cur;
		}
	}
}

void Menu_Processing::load_sound(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	config >> *sound_volume;
	config >> *music_volume;
}

void Menu_Processing::save_sound(std::string path) {
	std::ofstream fout;
	fout.open(path);
	fout << *sound_volume << "\n" << *music_volume;
	fout.close();
}

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
		std::string texture_name, button_name, name;
		b2Vec2 pos, scale;
		b2Vec2 axis_scale, slider_scale;
		int min_val, max_val, val;
		int use_window_cords;
		int character_size;
		file >> type;
		if (type == "Button")
			typenum = 1;
		if (type == "TextField")
			typenum = 2;
		if (type == "Slider")
			typenum = 3;
		if (type == "KeyboardField")
			typenum = 4;
		if (type == "ConstantText")
			typenum = 5;
		switch (typenum) {
		case 1:
			file >> button_name;
			file >> texture_name;
			file >> use_window_cords >> pos.x >> pos.y >> scale.x >> scale.y;
			object->add_button(i, texture_name, pos, use_window_cords, scale, sf::Color::White, mouse_pos);
			name_to_id[button_name] = i;
			break;
		case 2:
			file >> texture_name;
			file >> use_window_cords >> pos.x >> pos.y;
			file >> character_size;
			object->add_text_field(i, text_fields_strings[i], texture_name, pos, use_window_cords, character_size, sf::Color::White,
				1, mouse_pos, keyboard);
			break;
		case 3:
			file >> name;
			file >> use_window_cords >> pos.x >> pos.y;
			file >> axis_scale.x >> axis_scale.y >> slider_scale.x >> slider_scale.y;
			file >> min_val >> max_val >> val;
			object->add_slider(i, pos, use_window_cords, axis_scale, slider_scale,
				min_val, max_val, val, mouse_pos);
			name_to_id[name] = i;
			break;
		case 4:
			file >> texture_name;
			file >> use_window_cords >> pos.x >> pos.y;
			file >> character_size;
			object->add_keyboard_field(i, text_fields_strings[i], texture_name, pos, use_window_cords, character_size, sf::Color::White,
				1, mouse_pos, keyboard);
			break;
		case 5:
			file >> use_window_cords >> pos.x >> pos.y;
			file >> character_size;
			object->add_constant_text(i, text_fields_strings[i], pos, use_window_cords, character_size, sf::Color::White,
				2, mouse_pos, keyboard);
			break;
		default:
			i--;
			current_id--;
			break;
		}
	}
}

void Menu_Processing::init(Draw* draw_, b2Vec2* mouse_pos_,
	aux::Keyboard* keyboard_, bool* reload_,
	int* sound_volume_, int* music_volume_) {
	draw = draw_;
	keyboard = keyboard_;
	mouse_pos = mouse_pos_;
	reload = reload_;
	sound_volume = sound_volume_;
	music_volume = music_volume_;
	load_sound("sound_settings.conf");
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
	text_fields_strings[current_id] = "Server IP:";
	text_fields_strings[current_id + 2] = "Port:";
	text_fields_strings[current_id + 4] = "ID:";
	text_fields_strings[current_id + 6] = "Name:";
	load_config("client_config.conf", &text_fields_strings[current_id + 1], &text_fields_strings[current_id + 3],
		&text_fields_strings[current_id + 5], &text_fields_strings[current_id + 7]);
	menus.push_back(&config_menu);
	init_menu("menu_configs/client_config.conf", &config_menu);
	// set settigs menu 
	settings_menu.set_draw(draw);
	settings_menu.set_active(0);
	settings_menu.set_events(&events);
	settings_menu.set_sliders_vals(&sliders_vals);
	settings_menu.set_text_fields_strings(&text_fields_strings);
	menus.push_back(&settings_menu);
	init_menu("menu_configs/settings.conf", &settings_menu);
	// sound menu settings
	sound_menu.set_draw(draw);
	sound_menu.set_active(0);
	sound_menu.set_events(&events);
	sound_menu.set_sliders_vals(&sliders_vals);
	sound_menu.set_text_fields_strings(&text_fields_strings);
	text_fields_strings[current_id] = "Sound Volume:";
	text_fields_strings[current_id + 2] = "Music Volume:";
	sliders_vals[current_id + 1] = *sound_volume;
	sliders_vals[current_id + 3] = *music_volume;
	menus.push_back(&sound_menu);
	init_menu("menu_configs/sound.conf", &sound_menu);
	// set keys menu fields
	keys_menu.set_draw(draw);
	keys_menu.set_active(0);
	keys_menu.set_events(&events);
	keys_menu.set_sliders_vals(&sliders_vals);
	keys_menu.set_text_fields_strings(&text_fields_strings);
	menus.push_back(&keys_menu);
	init_menu("menu_configs/keys.conf", &keys_menu);
	load_keys("keys.conf", &keys_menu_vec, &keys_menu, { 0, -300 }, -30, { 100, 50 }, 20);
	name_to_id["ApplySounds"] = ++current_id;
	name_to_id["Apply"] = ++current_id;
}

void Menu_Processing::step() {
	if (active) {
		for (auto menu : menus) {
			menu->step();
		}
		*sound_volume = sliders_vals[name_to_id["SoundVolume"]];
		*music_volume = sliders_vals[name_to_id["MusicVolume"]];
		disactivated = 1;
	}
	if (!active) {
		if (disactivated) {
			events.push(name_to_id["ApplyClientConfig"]);
			events.push(name_to_id["ApplyKeys"]);
		}
		while (keyboard->text_entered->size())
			keyboard->text_entered->pop();
		disactivated = 0;
	}
	while (!events.empty()) {
		if (name_to_id["NewGame"] == events.front()) { // New game button
			active = 0;
		}
		if (name_to_id["About"] == events.front()) { // About button
			std::cout << "well, it works";
		}
		if (name_to_id["Settings"] == events.front()) { // Settings button
			sound_menu.set_active(0);
			main_menu.set_active(0);
			settings_menu.set_active(1);
			config_menu.set_active(1);
		}
		if (name_to_id["Exit"] == events.front()) { // Exit button
			draw->get_window()->close();
		}
		if (name_to_id["ApplyClientConfig"] == events.front()) { // Apply button
			save_config("client_config.conf", text_fields_strings[6], atoi(text_fields_strings[8].c_str()),
				atoi(text_fields_strings[10].c_str()), text_fields_strings[12]);
			*reload = 1;
		}
		if (name_to_id["Back"] == events.front()) { // Back button
			main_menu.set_active(1);
			settings_menu.set_active(0);
			config_menu.set_active(0);
			keys_menu.set_active(0);
			sound_menu.set_active(0);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["Main"] == events.front()) { // Main button
			config_menu.set_active(1);
			keys_menu.set_active(0);
			sound_menu.set_active(0);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["Control"] == events.front()) { // Control button
			config_menu.set_active(0);
			sound_menu.set_active(0);
			keys_menu.set_active(1);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["ApplyKeys"] == events.front()) { // Apply button
			save_keys("keys.conf", keys_menu_vec);
			*reload = 1;
		}
		if (name_to_id["Sound"] == events.front()) {
			config_menu.set_active(0);
			keys_menu.set_active(0);
			sound_menu.set_active(1);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["Apply"] == events.front()) {
			events.push(name_to_id["ApplyKeys"]);
			events.push(name_to_id["ApplyClientConfig"]);
			events.push(name_to_id["ApplySound"]);
		}
		if (name_to_id["ApplySound"] == events.front()) {
			save_sound("sound_settings.conf");
		}
		events.pop();
	}
}
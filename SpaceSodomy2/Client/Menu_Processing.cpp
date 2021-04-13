#include "Menu_Processing.h"

Menu_Processing::Menu_Processing() {}

void Menu_Processing::init_hull(std::string name, int hp, float mass, float radius,
	int stamina, int stamina_recovery, Menu* hull) {
	hull->add_image(++current_id, name + "-hull", b2Vec2(-150, 150), 7, b2Vec2(200, 200), mouse_pos, 0);
	hull->add_constant_text(++current_id, "Name: " + name, b2Vec2(-300, 300), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	hull->add_constant_text(++current_id, "Health: " + std::to_string(hp), b2Vec2(-300, 325), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	hull->add_constant_text(++current_id, "Mass: " + std::to_string(mass), b2Vec2(-300, 350), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	hull->add_constant_text(++current_id, "Radius: " + std::to_string(radius), b2Vec2(-300, 375), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	hull->add_constant_text(++current_id, "Stamina: " + std::to_string(stamina), b2Vec2(-300, 400), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	hull->add_constant_text(++current_id, "Stamina recovery: " + std::to_string(stamina_recovery), b2Vec2(-300, 425), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	menus.push_back(hull);
}
void Menu_Processing::init_hull_menu(b2Vec2 pos, std::string path_to_hulls_description) {
	int cur_but_id = 0;
	std::ifstream file_to_comment(path_to_hulls_description);
	std::stringstream config = aux::comment(file_to_comment);
	int hp, stamina, stamina_recovery;
	float mass, radius;
	std::string name, next;
	while (next != "HULL")
		config >> next;
	while (next == "HULL") {
		config >> name >> next;
		name_to_id[name + "-hull"] = current_id;
		hull_menu.add_button(current_id++, name + "-hull", pos + b2Vec2(150 * (cur_but_id % 3), 150 * (cur_but_id / 3)),
			1, { 100, 100 }, sf::Color::White, mouse_pos, 0);
		hull_menu.add_constant_text(current_id++, name, pos + b2Vec2(150 * (cur_but_id % 3), 150 * (cur_but_id / 3) + 65),
			1, 20, sf::Color::White, 0, mouse_pos, keyboard);
		cur_but_id++;
		while (next != "END") {
			if (next == "HP") {
				config >> hp;
			}
			if (next == "MASS") {
				config >> mass;
			}
			if (next == "RADIUS") {
				config >> radius;
			}
			if (next == "STAMINA") {
				config >> stamina;
			}
			if (next == "STAMINA_RECOVERY") {
				config >> stamina_recovery;
			}
			config >> next;
		}
		hulls[name].set_draw(draw);
		hulls[name].set_active(0);
		hulls[name].set_events(&events);
		hulls[name].set_sliders_vals(&sliders_vals);
		hulls[name].set_text_fields_strings(&text_fields_strings);
		init_hull(name, hp, mass, radius,
			stamina, stamina_recovery, &hulls[name]);
		config >> next;
	}
}

void Menu_Processing::close_settings_menus() {
	config_menu.set_active(0);
	keys_menu.set_active(0);
	sound_menu.set_active(0);
	gun_menu.set_active(0);
	hull_menu.set_active(0);
	HUD_menu.set_active(0);
	for (auto it = guns.begin(); it != guns.end(); it++)
		guns[it->first].set_active(0);
	for (auto it = hulls.begin(); it != hulls.end(); it++)
		hulls[it->first].set_active(0);
}

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

void Menu_Processing::load_aim_settings(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	double next;
	config >> next;
	game->set_aim_conf(next);
	config >> next;
	game->set_aim_opacity(next);
}

void Menu_Processing::save_aim_settings(std::string path) {
	std::ofstream fout;
	fout.open(path);
	fout << game->get_aim_conf() << "\n" << game->get_aim_opacity();
	fout.close();
}

void Menu_Processing::load_sound(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	double volume;
	config >> volume;
	game->get_audio()->set_sound_volume(volume);
	config >> volume;
	game->get_audio()->set_music_volume(volume);
}

void Menu_Processing::save_sound(std::string path) {
	std::ofstream fout;
	fout.open(path);
	fout << game->get_audio()->get_sound_volume() << "\n" << game->get_audio()->get_music_volume();
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
	if ((*id_) == "0") {
		*id_ = std::to_string(aux::random_int(1, 100000));
		save_config(path, *address_, atoi(port_->c_str()), atoi(id_->c_str()), *name_);
	}
}

void Menu_Processing::init_menu(std::string path_, Menu* object) {
	// uncomment config file
	std::ifstream comment_file(path_);
	std::stringstream file = aux::comment(comment_file);
	for (int i = current_id; !file.eof(); i++) {
		current_id++;
		std::string type, next;
		int typenum = 0, align = 0;
		std::string texture_name, button_name, name;
		b2Vec2 pos, scale;
		b2Vec2 axis_scale, slider_scale;
		int min_val, max_val, val;
		int use_window_cords, use_image_scale, critical;
		int character_size;
		sf::Color back_color, front_color;
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
		if (type == "Image")
			typenum = 6;
		if (type == "Bar")
			typenum = 7;
		switch (typenum) {
		case 1:
			file >> next;
			button_name = "default";
			texture_name = "NewGame";
			use_window_cords = 0;
			pos = { 0, 0 };
			use_image_scale = 1;
			scale = { 1, 1 };
			while (next != "END") {
				if (next == "NAME")
					file >> button_name;
				if (next == "TEXTURE_NAME")
					file >> texture_name;
				if (next == "USE_WINDOWS_CORDS")
					file >> use_window_cords;
				if (next == "POS")
					file >> pos.x >> pos.y;
				if (next == "SCALE") {
					file >> scale.x >> scale.y;
					use_image_scale = 0;
				}
				file >> next;
			}
			if (scale.x == -1)
				scale.x = sf::VideoMode::getDesktopMode().width;
			if (scale.y == -1)
				sf::VideoMode::getDesktopMode().height;
			object->add_button(i, texture_name, pos, use_window_cords, scale, sf::Color::White, mouse_pos, use_image_scale);
			name_to_id[button_name] = i;
			break;
		case 2:
			texture_name = "TextField";
			use_window_cords = 0;
			pos = { 0,0 };
			character_size = 20;
			align = 1;
			file >> next;
			while (next != "END") {
				if (next == "TEXTURE_NAME")
					file >> texture_name;
				if (next == "USE_WINDOWS_CORDS")
					file >> use_window_cords;
				if (next == "POS")
					file >> pos.x >> pos.y;
				if (next == "CHARACTER_SIZE")
					file >> character_size;
				if (next == "ALIGN")
					file >> align;
				file >> next;
			}
			object->add_text_field(i, text_fields_strings[i], texture_name, pos, use_window_cords, character_size, sf::Color::White,
				align, mouse_pos, keyboard);
			break;
		case 3:
			name = "default";
			use_window_cords = 0;
			pos = { 0,0 };
			axis_scale = { 100, 5 };
			slider_scale = {10, 10};
			min_val = 0;
			max_val = 100;
			file >> next;
			while (next != "END") {
				if (next == "NAME")
					file >> name;
				if (next == "USE_WINDOWS_CORDS")
					file >> use_window_cords;
				if (next == "POS")
					file >> pos.x >> pos.y;
				if (next == "AXIS_SCALE")
					file >> axis_scale.x >> axis_scale.y;
				if (next == "SLIDER_SCALE")
					file >> slider_scale.x >> slider_scale.y;
				if (next == "MIN")
					file >> min_val;
				if (next == "MAX")
					file >> max_val;
				file >> next;
			}
			if (axis_scale.x == -1)
				scale.x = sf::VideoMode::getDesktopMode().width;
			if (axis_scale.y == -1)
				axis_scale.y = sf::VideoMode::getDesktopMode().height;
			if (slider_scale.x == -1)
				scale.x = sf::VideoMode::getDesktopMode().width;
			if (slider_scale.y == -1)
				slider_scale.y = sf::VideoMode::getDesktopMode().height;
			object->add_slider(i, pos, use_window_cords, axis_scale, slider_scale,
				min_val, max_val, sliders_vals[i], mouse_pos);
			name_to_id[name] = i;
			break;
		case 4:
			texture_name = "TextField";
			use_window_cords = 0;
			pos = { 0,0 };
			character_size = 20;
			align = 1;
			file >> next;
			while (next != "END") {
				if (next == "TEXTURE_NAME")
					file >> texture_name;
				if (next == "USE_WINDOWS_CORDS")
					file >> use_window_cords;
				if (next == "POS")
					file >> pos.x >> pos.y;
				if (next == "CHARACTER_SIZE")
					file >> character_size;
				if (next == "ALIGN")
					file >> align;
				file >> next;
			}
			object->add_keyboard_field(i, text_fields_strings[i], texture_name, pos, use_window_cords, character_size, sf::Color::White,
				align, mouse_pos, keyboard);
			break;
		case 5:
			use_window_cords = 0;
			pos = { 0, 0 };
			character_size = 20;
			file >> next;
			align = 2;
			while (next != "END") {
				if (next == "USE_WINDOWS_CORDS")
					file >> use_window_cords;
				if (next == "POS")
					file >> pos.x >> pos.y;
				if (next == "CHARACTER_SIZE")
					file >> character_size;
				if (next == "ALIGN")
					file >> align;
				file >> next;
			}
			if (scale.x == -1)
				scale.x = sf::VideoMode::getDesktopMode().width;
			if (scale.y == -1)
				scale.y = sf::VideoMode::getDesktopMode().height;
			object->add_constant_text(i, text_fields_strings[i], pos, use_window_cords, character_size, sf::Color::White,
				align, mouse_pos, keyboard);
			break;
		case 6:
			file >> next;
			texture_name = "NewGame";
			use_window_cords = 0;
			pos = { 0, 0 };
			use_image_scale = 1;
			scale = { 1, 1 };
			while (next != "END") {
				if (next == "TEXTURE_NAME")
					file >> texture_name;
				if (next == "USE_WINDOWS_CORDS")
					file >> use_window_cords;
				if (next == "POS")
					file >> pos.x >> pos.y;
				if (next == "SCALE") {
					file >> scale.x >> scale.y;
					use_image_scale = 0;
				}
				file >> next;
			}
			if (scale.x == -1)
				scale.x = sf::VideoMode::getDesktopMode().width;
			if (scale.y == -1)
				scale.y = sf::VideoMode::getDesktopMode().height;
			object->add_image(i, texture_name, pos, use_window_cords, scale, mouse_pos, use_image_scale);
			break;
		case 7:
			file >> next;
			name = "default";
			use_window_cords = 4;
			pos = { 0, 0 };
			scale = { 0, 0 };
			character_size = 0;
			back_color = sf::Color(140, 140, 140, 255);
			front_color = sf::Color(200, 200, 200, 255);
			critical = 0;
			while (next != "END") {
				if (next == "NAME")
					file >> name;
				if (next == "USE_WINDOWS_CORDS")
					file >> use_window_cords;
				if (next == "POS")
					file >> pos.x >> pos.y;
				if (next == "SCALE")
					file >> scale.x >> scale.y;
				if (next == "CRITICAL")
					file >> critical;
				if (next == "CHARACTER_SIZE")
					file >> character_size;
				if (next == "BACK_COLOR")
					file >> back_color.r >> back_color.g >> back_color.b >> back_color.a;
				if (next == "FRONT_COLOR")
					file >> front_color.r >> front_color.g >> front_color.b >> front_color.a;
				file >> next;
			}
			if (scale.x == -1)
				scale.x = sf::VideoMode::getDesktopMode().width;
			if (scale.y == -1)
				scale.y = sf::VideoMode::getDesktopMode().height;
			name_to_id[name] = i;
			object->add_bar(i, use_window_cords, pos, scale, character_size, front_color, back_color, bars_vals[i].second, &bars_vals[i].first, critical);
			break;
		default:
			i--;
			current_id--;
			break;
		}
	}
}

void Menu_Processing::init_gun(std::string name, int damage, float recharge, int stamina_consumption, float projectile_mass,
	float projectile_radius, int projectile_vel, Menu* gun) {
	gun->add_image(++current_id, name + "-gun", b2Vec2(-150, 150), 7, b2Vec2(200, 200), mouse_pos, 0);
	gun->add_constant_text(++current_id, "Name: " + name, b2Vec2(-300, 300), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Damage: " + std::to_string(damage), b2Vec2(-300, 325), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Recharge: " + std::to_string(recharge), b2Vec2(-300, 350), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Stamina consumption: " + std::to_string(stamina_consumption), b2Vec2(-300, 375), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Bullet mass: " + std::to_string(projectile_mass), b2Vec2(-300, 400), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Bullet radius: " + std::to_string(projectile_radius), b2Vec2(-300, 425), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Bullet velocity: " + std::to_string(projectile_vel), b2Vec2(-300, 450), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	menus.push_back(gun);
}

void Menu_Processing::init_gun_menu(b2Vec2 pos, std::string path_to_guns_description) {
	int cur_but_id = 0;
	std::ifstream file_to_comment(path_to_guns_description);
	std::stringstream config = aux::comment(file_to_comment);
	while (!config.eof()) {
		int damage, stamina_consumption, projectile_vel;
		float recharge, projectile_mass, projectile_radius;
		std::string name, next;
		config >> next;
		while (next == "GUN") {
			config >> name >> next;
			name_to_id[name + "-gun"] = current_id;
			gun_menu.add_button(current_id++, name + "-gun", pos + b2Vec2(150 * (cur_but_id % 3), 150 * (cur_but_id / 3)),
				1, { 100, 100 }, sf::Color::White, mouse_pos, 0);
			gun_menu.add_constant_text(current_id++, name, pos + b2Vec2(150 * (cur_but_id % 3), 150 * (cur_but_id / 3) + 65),
				1, 20, sf::Color::White, 0, mouse_pos, keyboard);
			cur_but_id++;
			while (next != "END") {
				if (next == "DAMAGE") {
					config >> damage;
				}
				if (next == "RECHARGE") {
					config >> recharge;
				}
				if (next == "STAMINA_CONSUMPTION") {
					config >> stamina_consumption;
				}
				if (next == "PROJECTILE_MASS") {
					config >> projectile_mass;
				}
				if (next == "PROJECTILE_RADIUS") {
					config >> projectile_radius;
				}
				if (next == "PROJECTILE_VEL") {
					config >> projectile_vel;
				}
				config >> next;
			}
			guns[name].set_draw(draw);
			guns[name].set_active(0);
			guns[name].set_events(&events);
			guns[name].set_sliders_vals(&sliders_vals);
			guns[name].set_text_fields_strings(&text_fields_strings);
			init_gun(name, damage, recharge, stamina_consumption, projectile_mass,
				projectile_radius, projectile_vel, &guns[name]);
			config >> next;
		}
	}
}

void Menu_Processing::init(Draw* draw_, b2Vec2* mouse_pos_,
	aux::Keyboard* keyboard_, bool* reload_,
	Game_Client* game_, Counter* replay_frame_) {
	game = game_;
	draw = draw_;
	keyboard = keyboard_;
	mouse_pos = mouse_pos_;
	reload = reload_;
	replay_frame = replay_frame_;
	load_sound("sound_settings.conf");
	load_aim_settings("HUD_settings.conf");
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
	menus.push_back(&sound_menu);
	sliders_vals[current_id + 1] = game->get_audio()->get_sound_volume();
	sliders_vals[current_id + 3] = game->get_audio()->get_music_volume();
	init_menu("menu_configs/sound.conf", &sound_menu);
	// set keys menu fields
	keys_menu.set_draw(draw);
	keys_menu.set_active(0);
	keys_menu.set_events(&events);
	keys_menu.set_sliders_vals(&sliders_vals);
	keys_menu.set_text_fields_strings(&text_fields_strings);
	menus.push_back(&keys_menu);
	init_menu("menu_configs/keys.conf", &keys_menu);
	load_keys("keys.conf", &keys_menu_vec, &keys_menu, { 0, -350 }, -30, { 100, 50 }, 20);

	name_to_id["ApplySound"] = current_id++;
	name_to_id["Apply"] = current_id++;
	name_to_id["ApplySetup"] = current_id++;
	name_to_id["ApplyKeys"] = current_id++;
	name_to_id["ApplyClientConfig"] = current_id++;
	name_to_id["ApplyHUD"] = current_id++;
	// set gun menu fields
	gun_menu.set_draw(draw);
	gun_menu.set_active(0);
	gun_menu.set_events(&events);
	gun_menu.set_sliders_vals(&sliders_vals);
	gun_menu.set_text_fields_strings(&text_fields_strings);
	menus.push_back(&gun_menu);
	init_gun_menu(b2Vec2(300, 100), "parameters.conf");
	// set hull menu fields
	hull_menu.set_draw(draw);
	hull_menu.set_active(0);
	hull_menu.set_events(&events);
	hull_menu.set_sliders_vals(&sliders_vals);
	hull_menu.set_text_fields_strings(&text_fields_strings);
	menus.push_back(&hull_menu);
	init_hull_menu(b2Vec2(300, 100), "parameters.conf");
	// set HUD menu fields
	HUD_menu.set_draw(draw);
	HUD_menu.set_active(0);
	HUD_menu.set_events(&events);
	HUD_menu.set_sliders_vals(&sliders_vals);
	HUD_menu.set_text_fields_strings(&text_fields_strings);
	text_fields_strings[current_id] = "Aim Configuration:";
	text_fields_strings[current_id + 2] = "Aim Opacity:";
	sliders_vals[current_id + 1] = game->get_aim_conf();
	sliders_vals[current_id + 3] = game->get_aim_opacity();
	init_menu("menu_configs/HUD.conf", &HUD_menu);
	menus.push_back(&HUD_menu);
	// set replay_menu
	replay_menu.set_draw(draw);
	replay_menu.set_active(0);
	replay_menu.set_events(&events);
	replay_menu.set_sliders_vals(&sliders_vals);
	sliders_vals[current_id] = 0;
	bars_vals[current_id + 1] = { 50, 100000 };
	init_menu("menu_configs/replay.conf", &replay_menu);
	menus.push_back(&replay_menu);
	replay_frame->set_change_vel(1);
	replay_frame->set_max(100000);
}

void Menu_Processing::step() {
	replay_frame->step(1);
	if (active) {
		game->get_draw()->fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(game->get_draw()->get_window()->getSize())),
			sf::Color(0, 0, 0, 90), 0);
		text_field_active = 0;
		sliders_vals[name_to_id["ReplaySlider"]] = replay_frame->get();
		for (auto menu : menus) {
			menu->step();
			text_field_active |= menu->text_field_active;
		}
		game->get_audio()->set_sound_volume(sliders_vals[name_to_id["SoundVolume"]]);
		game->get_audio()->set_music_volume(sliders_vals[name_to_id["MusicVolume"]]);
		game->set_aim_conf(sliders_vals[name_to_id["AimConfiguration"]]);
		game->set_aim_opacity(sliders_vals[name_to_id["AimOpacity"]]);
		bars_vals[name_to_id["ReplayBar"]].first = sliders_vals[name_to_id["ReplaySlider"]];
		replay_frame->set(sliders_vals[name_to_id["ReplaySlider"]]);
		disactivated = 1;
	}
	if (!active) {
		if (disactivated) {
			events.push(name_to_id["Apply"]);
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
			close_settings_menus();
			main_menu.set_active(0);
			settings_menu.set_active(1);
			config_menu.set_active(1);
		}
		if (name_to_id["Exit"] == events.front()) { // Exit button
			draw->get_window()->close();
		}
		if (name_to_id["ApplyClientConfig"] == events.front()) { // Apply button
			save_config("client_config.conf", text_fields_strings[7], atoi(text_fields_strings[9].c_str()),
				atoi(text_fields_strings[11].c_str()), text_fields_strings[13]);
			*reload = 1;
		}
		if (name_to_id["Back"] == events.front()) { // Back button
			main_menu.set_active(1);
			settings_menu.set_active(0);
			close_settings_menus();
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["Main"] == events.front()) { // Main button
			close_settings_menus();
			config_menu.set_active(1);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["Control"] == events.front()) { // Control button
			close_settings_menus();
			keys_menu.set_active(1);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["ApplyKeys"] == events.front()) { // Apply button
			save_keys("keys.conf", keys_menu_vec);
			*reload = 1;
		}
		if (name_to_id["Sound"] == events.front()) {
			close_settings_menus();
			sound_menu.set_active(1);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["Apply"] == events.front()) {
			events.push(name_to_id["ApplyKeys"]);
			events.push(name_to_id["ApplyClientConfig"]);
			events.push(name_to_id["ApplySound"]);
			events.push(name_to_id["ApplySetup"]);
			events.push(name_to_id["ApplyHUD"]);
		}
		if (name_to_id["ApplySound"] == events.front()) {
			save_sound("sound_settings.conf");
		}
		if (name_to_id["Gun"] == events.front()) {
			events.push(name_to_id[game->get_gun_name() + "-gun"]);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["Hull"] == events.front()) {
			events.push(name_to_id[game->get_hull_name() + "-hull"]);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["ApplyHUD"] == events.front()) {
			save_aim_settings("HUD_settings.conf");
		}
		if (name_to_id["HUD"] == events.front()) {
			close_settings_menus();
			HUD_menu.set_active(1);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["Replay"] == events.front()) {
			main_menu.set_active(0);
			replay_menu.set_active(1);
		}
		for (auto it = guns.begin(); it != guns.end(); it++) {
			if (name_to_id[it->first + "-gun"] == events.front()) {
				game->set_gun_name(it->first);
				close_settings_menus();
				gun_menu.set_active(1);
				guns[game->get_gun_name()].set_active(1);
				events.push(name_to_id["Apply"]);
			}
		}
		for (auto it = hulls.begin(); it != hulls.end(); it++) {
			if (name_to_id[it->first + "-hull"] == events.front()) {
				game->set_hull_name(it->first);
				close_settings_menus();
				hull_menu.set_active(1);
				hulls[game->get_hull_name()].set_active(1);
				events.push(name_to_id["Apply"]);
			}
		}
		if (name_to_id["ApplySetup"] == events.front()) {
			//set_current_gun("setup.conf", cur_gun);
			game->save_setup("setup.conf");
		}
		events.pop();
	}
}
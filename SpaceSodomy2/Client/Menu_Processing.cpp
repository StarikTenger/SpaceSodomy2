#include "Menu_Processing.h"

Menu_Processing::Menu_Processing() {}

void Menu_Processing::init_hull(std::string name, int hp, float mass, float radius,
	int stamina, int stamina_recovery, Menu* hull) {
	hull->add_image(++current_id, name + "-hull", b2Vec2(-150, 150), 7, b2Vec2(200, 200), mouse_pos, 0);
	hull->add_image(++current_id, name + "-hull-colors", b2Vec2(-150, 150), 7, b2Vec2(200, 200), mouse_pos, 0);
	hull->add_constant_text(++current_id, "Name: " + name, b2Vec2(-300, 300), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	hull->add_constant_text(++current_id, "Health: " + std::to_string(hp), b2Vec2(-300, 325), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	hull->add_constant_text(++current_id, "Mass: " + aux::float_to_string(mass, 2), b2Vec2(-300, 350), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	hull->add_constant_text(++current_id, "Radius: " + aux::float_to_string(radius, 2), b2Vec2(-300, 375), 7, 20,
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
	while (!config.eof()) {
		config >> next;
		while (next == "HULL") {
			config >> name >> next;
			name_to_id[name + "-hull"] = current_id;
			hull_menu.add_button(current_id++, name + "-hull", pos + b2Vec2(150 * (cur_but_id % 3), 150 * (cur_but_id / 3)),
				1, { 100, 100 }, sf::Color::White, mouse_pos, 0);
			hull_menu.add_button(current_id++, name + "-hull-colors", pos + b2Vec2(150 * (cur_but_id % 3), 150 * (cur_but_id / 3)),
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
			init_hull(name, hp, mass, radius,
				stamina, stamina_recovery, &hulls[name]);
			config >> next;
		}
	}
}

void Menu_Processing::init_gun(std::string name, int damage, float recharge, int stamina_cost, float projectile_mass,
	float projectile_radius, int projectile_vel, Menu* gun) {
	gun->add_image(++current_id, name + "-gun", b2Vec2(-150, 150), 7, b2Vec2(200, 200), mouse_pos, 0);
	gun->add_constant_text(++current_id, "Name: " + name, b2Vec2(-300, 300), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Damage: " + std::to_string(damage), b2Vec2(-300, 325), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Recharge: " + aux::float_to_string(recharge, 2), b2Vec2(-300, 350), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Stamina consumption: " + std::to_string(stamina_cost), b2Vec2(-300, 375), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Bullet mass: " + aux::float_to_string(projectile_mass, 2), b2Vec2(-300, 400), 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	gun->add_constant_text(++current_id, "Bullet radius: " + aux::float_to_string(projectile_radius, 2), b2Vec2(-300, 425), 7, 20,
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
		int damage, stamina_cost, projectile_vel;
		float recharge, projectile_mass, projectile_radius;
		std::string name, next;
		config >> next;
		while (next == "GUN") {
			config >> name >> next;
			name_to_id[name + "-gun"] = current_id;
			gun_menu.add_button(current_id++, name + "-gun", pos + b2Vec2(300 * (cur_but_id % 3), 300 * (cur_but_id / 3)),
				1, { 200, 200 }, sf::Color::White, mouse_pos, 0);
			gun_menu.add_constant_text(current_id++, name, pos + b2Vec2(300 * (cur_but_id % 3), 300 * (cur_but_id / 3) + 65),
				1, 40, sf::Color::White, 0, mouse_pos, keyboard);
			cur_but_id++;
			while (next != "END") {
				if (next == "DAMAGE") {
					config >> damage;
				}
				if (next == "RECHARGE") {
					config >> recharge;
				}
				if (next == "STAMINA_CONSUMPTION") {
					config >> stamina_cost;
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
			init_gun(name, damage, recharge, stamina_cost, projectile_mass,
				projectile_radius, projectile_vel, &guns[name]);
			config >> next;
		}
	}
}


void Menu_Processing::init_module(std::string name, Menu* module, b2Vec2 add_pos) {
	module->add_button(++current_id, name + "-module", b2Vec2(-150, 150) + add_pos, 7, b2Vec2(200, 200), sf::Color::White, mouse_pos, 0);
	module->add_constant_text(++current_id, "Name: " + name, b2Vec2(-300, 300) + add_pos, 7, 20,
		sf::Color::White, 1, mouse_pos, keyboard);
	menus.push_back(module);
}

void Menu_Processing::init_modules_menu(b2Vec2 pos, std::string path_to_modules_description) {
	int cur_but_id = 0;
	std::ifstream file_to_comment(path_to_modules_description);
	std::stringstream config = aux::comment(file_to_comment);
	while (!config.eof()) {
		std::string name, next;
		config >> next;
		while (next == "MODULE") {
			config >> name >> next;
			name_to_id[name + "-module"] = current_id;
			modules_menu.add_button(current_id++, name + "-module", pos + b2Vec2(150 * (cur_but_id % 3), 150 * (cur_but_id / 3)),
				1, { 100, 100 }, sf::Color::White, mouse_pos, 0);
			modules_menu.add_constant_text(current_id++, name, pos + b2Vec2(150 * (cur_but_id % 3), 150 * (cur_but_id / 3) + 65),
				1, 20, sf::Color::White, 0, mouse_pos, keyboard);
			cur_but_id++;
			while (next != "END") {
				config >> next;
			}
			modules[name + "1"].set_draw(draw);
			modules[name + "1"].set_active(0);
			modules[name + "1"].set_events(&events);
			name_to_id[name + "1"] = current_id + 1;
			init_module(name, &modules[name + "1"], {-210, 0});
			modules[name + "2"].set_draw(draw);
			modules[name + "2"].set_active(0);
			modules[name + "2"].set_events(&events);
			name_to_id[name + "2"] = current_id + 1;
			init_module(name, &modules[name + "2"], { 0, 0 });
			config >> next;
		}
	}
}

void Menu_Processing::close_settings_menus() {
	config_menu.set_active(0);
	keys_menu.set_active(0);
	sound_menu.set_active(0);
	gun_menu.set_active(0);
	hull_menu.set_active(0);
	HUD_menu.set_active(0);
	modules_menu.set_active(0);
	for (auto it = guns.begin(); it != guns.end(); it++)
		guns[it->first].set_active(0);
	for (auto it = hulls.begin(); it != hulls.end(); it++)
		hulls[it->first].set_active(0);
	for (auto it = modules.begin(); it != modules.end(); it++) {
		modules[it->first].set_active(0);
	}
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
void Menu_Processing::load_keys(std::string path, std::vector<std::vector<std::string*>>* keys, tgui::Gui& gui,
	b2Vec2 pos, float name_indent, b2Vec2 indent, int character_size) {
	int k = 0;
	auto control_panel = gui.get<tgui::Panel>("ControlPanel");
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	for (int i = 0; !config.eof(); i++) {
		if (keys->size() == i)
			keys->push_back(std::vector<std::string*>());
		std::string cur, cur_name;
		config >> cur_name >> cur;
		for (int j = 0; !config.eof() && (cur != "END"); j++) {
			if (j == 0) {
				auto label = tgui::Label::create();
				label->setText(cur_name);
				auto size = tgui::Layout2d(
					tgui::Layout("30%"),
					tgui::Layout(20)
				);
				label->setSize(size);
				auto layout = tgui::Layout2d(
					tgui::Layout(std::to_string(5) + "%"),
					tgui::Layout(std::to_string(5 + 25 * i))
				);
				label->setPosition(layout);
				control_panel->add(label);
			}
			if (j == keys->operator[](i).size()) 
				keys->operator[](i).push_back(new std::string(cur));
			else
				*(keys->operator[](i)[j]) = cur;
			auto keybinding = KeybindingBox::create();
			keybinding->setText(cur);
			auto size = tgui::Layout2d(
				tgui::Layout("15%"),
				tgui::Layout(20)
			);
			keybinding->setSize(size);
			auto layout = tgui::Layout2d(
				tgui::Layout(std::to_string(40 + j * 20) + "%"),
				tgui::Layout(std::to_string(5 + 25 * i))
			);
			keybinding->setPosition(layout);
			control_panel->add(keybinding);
			std::string name = "keybinding" + std::to_string(k);
			keybinding->setWidgetName(name);
			keybinding->keys = keys;
			keybinding->reload = reload;
			k++;
			keybinding->onKeyPressed([=, &gui]() {
				auto key = gui.get<KeybindingBox>(name)->keys->operator[](i)[j];
				auto new_key = gui.get<KeybindingBox>(name)->getText().toStdString();
				*key = new_key;
				auto _reload = gui.get<KeybindingBox>(name)->reload;
				*_reload = true;
			});
			config >> cur;
		}
	}
}

void Menu_Processing::load_HUD_settings(std::string path, tgui::Gui &gui) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	double next;
	config >> next;
	game->set_aim_conf(next);
	gui.get<tgui::Slider>("AimConfiguration")->setValue(next);
	gui.get<tgui::Slider>("AimConfiguration")->onValueChange([=](int val) {
		game->set_aim_conf(val);
	});
	config >> next;
	game->set_aim_opacity(next);
	gui.get<tgui::Slider>("AimOpacity")->setValue(next);
	gui.get<tgui::Slider>("AimOpacity")->onValueChange([=](int val) {
		game->set_aim_opacity(val);
	});
	config >> next;
	game->set_network_information_active(next);
	gui.get<tgui::CheckBox>("NetworkInformation")->setChecked(next);
	gui.get<tgui::CheckBox>("NetworkInformation")->onChange([=](bool val) {
		game->set_network_information_active(val);
	});
}

void Menu_Processing::save_HUD_settings(std::string path) {
	std::ofstream fout;
	fout.open(path);
	fout << game->get_aim_conf() << "\n" << game->get_aim_opacity() << "\n" << game->get_network_information_active();
	fout.close();
}

void Menu_Processing::load_sound(std::string path, tgui::Gui &gui) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	auto music_slider = gui.get<tgui::Slider>("MusicSlider");
	auto sound_slider = gui.get<tgui::Slider>("SoundSlider");
	double volume;
	config >> volume;
	game->get_audio()->set_sound_volume(volume);
	sound_slider->setValue(volume);
	sound_slider->onValueChange([=](int new_val) {
		game->get_audio()->set_sound_volume(new_val);
		});
	config >> volume;
	game->get_audio()->set_music_volume(volume);
	music_slider->setValue(volume);
	music_slider->onValueChange([=](int new_val) {
		game->get_audio()->set_music_volume(new_val);
		});
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
		int discrete = 0;
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
		if (type == "CheckBox")
			typenum = 8;
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
			buttons[i] = object->add_button(i, texture_name, pos, use_window_cords, scale, sf::Color::White, mouse_pos, use_image_scale);
			name_to_id[button_name] = i;
			break;
		case 2:
			name = "default";
			texture_name = "TextField";
			use_window_cords = 0;
			pos = { 0,0 };
			character_size = 20;
			align = 1;
			file >> next;
			while (next != "END") {
				if (next == "NAME")
					file >> name;
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
			name_to_id[name] = i;
			text_fields[i] = object->add_text_field(i, "", texture_name, pos, use_window_cords, character_size, sf::Color::White,
				align, mouse_pos, keyboard);
			break;
		case 3:
			name = "default";
			use_window_cords = 0;
			pos = { 0,0 };
			axis_scale = { 100, 5 };
			slider_scale = {10, 10};
			min_val = 0;
			discrete = 0;
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
				if (next == "DISCRETE")
					file >> discrete;
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
			sliders[i] = object->add_slider(i, pos, use_window_cords, axis_scale, slider_scale,
				min_val, max_val, 0, mouse_pos);
			sliders[i]->set_discrete(discrete);
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
			keyboard_fields[i] = object->add_keyboard_field(i, "", texture_name, pos, use_window_cords, character_size, sf::Color::White,
				align, mouse_pos, keyboard);
			break;
		case 5:
			name = "default";
			use_window_cords = 0;
			pos = { 0, 0 };
			character_size = 20;
			file >> next;
			align = 2;
			while (next != "END") {
				if (next == "NAME")
					file >> name;
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
			name_to_id[name] = i;
			constant_texts[i] = object->add_constant_text(i, "", pos, use_window_cords, character_size, sf::Color::White,
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
			images[i] = object->add_image(i, texture_name, pos, use_window_cords, scale, mouse_pos, use_image_scale);
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
			bars[i] = object->add_bar(i, use_window_cords, pos, scale, character_size, front_color, back_color, 0, 0, critical);
			break;
		case 8:
			file >> next;
			name = "default";
			use_window_cords = 0;
			pos = { 0, 0 };
			while (next != "END") {
				if (next == "NAME")
					file >> name;
				if (next == "USE_WINDOWS_CORDS")
					file >> use_window_cords;
				if (next == "POS")
					file >> pos.x >> pos.y;
				file >> next;
			}
			name_to_id[name] = i;
			check_boxes[i] = object->add_check_box(i, pos, use_window_cords, mouse_pos);
			check_boxes[i]->set_use_picture_scale(0);
			check_boxes[i]->set_scale({ 30, 30 });
			check_boxes[i]->set_color(sf::Color::White);

		default:
			i--;
			current_id--;
			break;
		}
	}
}

void Menu_Processing::init_multiplayer_menu(std::string file_name, tgui::Gui& gui) {
	auto conf = gui.get<tgui::Group>("configuration.txt");

	auto gun_info = conf->get<tgui::ScrollablePanel>("GunInfo");
	auto hull_info = conf->get<tgui::ScrollablePanel>("HullInfo");
	auto module_info = conf->get<tgui::ScrollablePanel>("ModuleInfo");

	auto variants = conf->get<tgui::ScrollablePanel>("Variants");
	tgui::ScrollablePanel::Ptr gun_vars = tgui::ScrollablePanel::copy(variants),
		hull_vars = tgui::ScrollablePanel::copy(variants), module_vars = tgui::ScrollablePanel::copy(variants);

	conf->add(hull_vars);
	conf->add(module_vars);
	conf->add(gun_vars);

	hull_vars->setWidgetName("hull_vars");
	gun_vars->setWidgetName("gun_vars");
	module_vars->setWidgetName("module_vars");

	conf->get<tgui::Picture>("CurrentHull")->getRenderer()->setTexture(*draw->get_texture(game->get_hull_name() + "-hull"));
	conf->get<tgui::Picture>("CurrentHull")->onClick([=] {
		gun_vars->setVisible(0);
		hull_vars->setVisible(1);
		module_vars->setVisible(0);
	});

	conf->get<tgui::Picture>("CurrentGun")->getRenderer()->setTexture(*draw->get_texture(game->get_gun_name() + "-gun"));
	conf->get<tgui::Picture>("CurrentGun")->onClick([=] {
		gun_vars->setVisible(1);
		hull_vars->setVisible(0);
		module_vars->setVisible(0);
	});

	conf->get<tgui::Picture>("CurrentLeftModule")->getRenderer()->setTexture(*draw->get_texture(game->get_left_module_name() + "-module"));
	conf->get<tgui::Picture>("CurrentLeftModule")->onClick([=] {
		gun_vars->setVisible(0);
		hull_vars->setVisible(0);
		module_vars->setVisible(1);
	});

	conf->get<tgui::Picture>("CurrentRightModule")->getRenderer()->setTexture(*draw->get_texture(game->get_right_module_name() + "-module"));
	conf->get<tgui::Picture>("CurrentRightModule")->onClick([=] {
		gun_vars->setVisible(0);
		hull_vars->setVisible(0);
		module_vars->setVisible(1);
		});

	std::ifstream file_to_comment(file_name);
	std::stringstream config = aux::comment(file_to_comment);
	std::map<std::string, int> gun_maxes, hull_maxes, module_maxes;
	std::set<tgui::ScrollablePanel::Ptr> guns, hulls, modules;

	auto name_transform = [](std::string name) {
		std::transform(name.begin(), name.end(), name.begin(),
			[](unsigned char c) { 
				if (c == '_')
					return int(' ');
				else
					return std::tolower(c); 
			});
		name[0] = std::toupper(name[0]);
		return name;
	};
	auto close_info = [](tgui::Gui& gui) {
		auto widgets = gui.get<tgui::Group>("InfoGroup")->getWidgets();
		for (auto widget : widgets) {
			widget->setVisible(0);
		}
	};
	while (!config.eof()) {
		std::string name, next;
		config >> next;
		if (next == "GUN") {
			config >> name >> next;

			tgui::ScrollablePanel::Ptr new_gun = tgui::ScrollablePanel::copy(gun_info);
			new_gun->setWidgetName(name + "-gun");

			new_gun->get<tgui::Picture>("GunPreview")->getRenderer()
				->setTexture(*draw->get_texture(name + "-gun"));
			new_gun->get<tgui::Label>("GunName")->setText(name);

			auto pic = tgui::Picture::create();
			pic->getRenderer()->setTexture(*draw->get_texture(name + "-gun"));
			auto _game = game;
			pic->onClick([=, &gui, &close_info] {
				close_info(gui);
				new_gun->setVisible(1);

				_game->set_gun_name(new_gun->get<tgui::Label>("GunName")->getText().toStdString());
				gui.get<tgui::Picture>("CurrentGun")->getRenderer()
					->setTexture(new_gun->get<tgui::Picture>("GunPreview")->getRenderer()->getTexture());
			});
			gun_vars->add(pic);

			while (next != "END") {
				next = name_transform(next);
				float val, cval;
				int rval;
				config >> val;
				if (modf(val, &cval) > b2_epsilon) {
					rval = 100 * val;
				}
				auto val_bar = new_gun->get<tgui::ProgressBar>(next);
				if (val_bar != nullptr) {
					gun_maxes[next] = std::max(gun_maxes[next], rval);
					new_gun->get<tgui::ProgressBar>(next)->setMaximum(rval);
					new_gun->get<tgui::ProgressBar>(next)->setValue(rval);
				}
				config >> next;
			}

			guns.insert(new_gun);
			gui.get<tgui::Group>("InfoGroup")->add(new_gun);
		}
		if (next == "HULL") {
			config >> name >> next;

			tgui::ScrollablePanel::Ptr new_hull = tgui::ScrollablePanel::copy(hull_info);
			new_hull->setWidgetName(name + "-hull");

			new_hull->get<tgui::Picture>("HullPreview")->getRenderer()
				->setTexture(*draw->get_texture(name + "-hull"));
			new_hull->get<tgui::Label>("HullName")->setText(name);

			auto pic = tgui::Picture::create();
			pic->getRenderer()->setTexture(*draw->get_texture(name + "-hull"));
			auto _game = game;
			pic->onClick([=, &gui, &close_info] {
				close_info(gui);
				new_hull->setVisible(1);

				_game->set_hull_name(new_hull->get<tgui::Label>("HullName")->getText().toStdString());
				gui.get<tgui::Picture>("CurrentHull")->getRenderer()
					->setTexture(new_hull->get<tgui::Picture>("HullPreview")->getRenderer()->getTexture());
			});
			hull_vars->add(pic);

			while (next != "END") {
				next = name_transform(next);
				float val, cval;
				int rval;
				config >> val;
				if (modf(val, &cval) > b2_epsilon) {
					rval = 100 * val;
				}
				auto val_bar = new_hull->get<tgui::ProgressBar>(next);
				if (val_bar != nullptr) {
					hull_maxes[next] = std::max(hull_maxes[next], rval);
					new_hull->get<tgui::ProgressBar>(next)->setMaximum(rval);
					new_hull->get<tgui::ProgressBar>(next)->setValue(rval);
				}
				config >> next;
			}

			hulls.insert(new_hull);
			gui.get<tgui::Group>("InfoGroup")->add(new_hull);
		}
		if (next == "MODULE") {
			config >> name >> next;

			tgui::ScrollablePanel::Ptr new_module = tgui::ScrollablePanel::copy(module_info);
			new_module->setWidgetName(name + "-module");

			new_module->get<tgui::Picture>("ModulePreview")->getRenderer()
				->setTexture(*draw->get_texture(name + "-module"));
			new_module->get<tgui::Label>("ModuleName")->setText(name);

			auto pic = tgui::Picture::create();
			pic->getRenderer()->setTexture(*draw->get_texture(name + "-module"));
			auto _game = game;
			pic->onClick([=, &gui, &close_info] {
				close_info(gui);
				new_module->setVisible(1);

				if (module_num == 1) {
					_game->set_left_module_name(new_module->get<tgui::Label>("ModuleName")->getText().toStdString());
					gui.get<tgui::Picture>("CurrentLeftModule")->getRenderer()
						->setTexture(new_module->get<tgui::Picture>("ModulePreview")->getRenderer()->getTexture());
				}
				else {
					_game->set_right_module_name(new_module->get<tgui::Label>("ModuleName")->getText().toStdString());
					gui.get<tgui::Picture>("CurrentRightModule")->getRenderer()
						->setTexture(new_module->get<tgui::Picture>("ModulePreview")->getRenderer()->getTexture());
				}
			});
			module_vars->add(pic);

			while (next != "END") {
				next = name_transform(next);
				float val, cval;
				int rval;
				config >> val;
				if (modf(val, &cval) > b2_epsilon) {
					rval = 100 * val;
				}
				auto val_bar = new_module->get<tgui::ProgressBar>(next);
				if (val_bar != nullptr) {
					module_maxes[next] = std::max(module_maxes[next], rval);
					new_module->get<tgui::ProgressBar>(next)->setMaximum(rval);
					new_module->get<tgui::ProgressBar>(next)->setValue(rval);
				}
				config >> next;
			}

			modules.insert(new_module);
			gui.get<tgui::Group>("InfoGroup")->add(new_module);
		}
	}

	for (auto gun : guns) {
		for (auto param : gun_maxes) {
			gun->get<tgui::ProgressBar>(param.first)->setMaximum(param.second);
		}
	}

	for (auto hull : hulls) {
		for (auto param : hull_maxes) {
			hull->get<tgui::ProgressBar>(param.first)->setMaximum(param.second);
		}
	}

	for (auto module : modules) {
		for (auto param : module_maxes) {
			module->get<tgui::ProgressBar>(param.first)->setMaximum(param.second);
		}
	}
}

void Menu_Processing::init_tgui(tgui::Gui& gui) {
	auto load_widgets = [&gui](std::string file_name) {
		auto ans = tgui::Group::create();
		ans->loadWidgetsFromFile(file_name);
		ans->setVisible(false);
		gui.add(ans);
		ans->setWidgetName(file_name);
		return ans;
	};
	auto close_groups = [](tgui::Gui& gui) {
		auto wid = gui.getWidgets();
		for (int i = 0; i < wid.size(); i++) {
			wid[i]->setVisible(false);
		}
	};
	auto launch_replay = [=](tgui::Gui& gui, std::string name) {
		replay->set_replay_path("replays/" + name);
		replay->set_replay_active(1);
		gui.get<tgui::Label>("ReplayName")->setText(name);
		int max_time = replay->get_replay_frame()->get_max();
		gui.get<tgui::Label>("MaxTime")->setText(std::to_string(max_time / 3600) + ":" +
			std::to_string((max_time / 60) % 60) + ":" + std::to_string(max_time % 60));
		gui.get<tgui::Slider>("ReplaySlider")->setMaximum(max_time);
	};
	auto main_menu = load_widgets("main_menu.txt");
	main_menu->setVisible(true);
	auto HUD = load_widgets("HUD.txt");
	auto settings_menu = load_widgets("settings.txt");
	auto configuration_menu = load_widgets("configuration.txt");
	auto replay_menu = load_widgets("replay.txt");
	// Initializing main menu
	tgui::Button::Ptr replayButton = gui.get<tgui::Button>("Replay");
	replayButton->onClick([=, &gui, &close_groups] {
		close_groups(gui);
		launch_replay(gui, "example.ex");
		gui.get<tgui::Group>("replay.txt")->setVisible(true);
	});
	tgui::Button::Ptr settings = gui.get<tgui::Button>("Settings");
	settings->onClick([=, &gui, &close_groups] {
		close_groups(gui);
		gui.get<tgui::Group>("settings.txt")->setVisible(true);
	});
	auto multiplayer = gui.get<tgui::Button>("Multiplayer");
	multiplayer->onClick([=, &gui, &close_groups] {
		close_groups(gui);
		gui.get<tgui::Group>("configuration.txt")->setVisible(true);
	});
	auto exit = gui.get<tgui::Button>("Exit");
	exit->onClick([=] {
		draw->get_window()->close();
	});
	// Initializing replay menu
	auto select_replay_button = gui.get<tgui::Button>("SelectReplay");
	select_replay_button->onClick([=, &gui, &launch_replay] {
		auto choose_file = tgui::FileDialog::create("Open replay", "Open");
		choose_file->onFileSelect([=, &gui] {
			auto name = choose_file->getSelectedPaths()[0].getFilename().toStdString();
			launch_replay(gui, name);
			});
			gui.get<tgui::Group>("replay.txt")->add(choose_file);
	});
	auto spin_control = gui.get<tgui::SpinControl>("SpinControl");
	spin_control->onValueChange([=](float val) {
		replay->set_speed(val);
	});
	auto replay_slider = gui.get<tgui::Slider>("ReplaySlider");
	replay_slider->onValueChange([=](float val) {
		replay->get_replay_frame()->set(val);
	});
	auto replay_play_button = gui.get<tgui::Button>("ReplayPlay");
	replay_play_button->onPress([=, &gui] {
		replay->play_button(gui);
	});
	// Initializing control menu
	tgui::Button::Ptr control = gui.get<tgui::Button>("Control");
	control->onClick([&gui] {
		auto settings_panel = gui.get<tgui::Group>("SettingsPanel");
		for (auto widget : settings_panel->getWidgets()) {
			widget->setVisible(false);
		}
		gui.get<tgui::Group>("ControlPanel")->setVisible(true);
	});
	tgui::Button::Ptr audio = gui.get<tgui::Button>("Audio");
	audio->onClick([&gui] {
		auto settings_panel = gui.get<tgui::Group>("SettingsPanel");
		for (auto widget : settings_panel->getWidgets()) {
			widget->setVisible(false);
		}
		gui.get<tgui::Group>("AudioPanel")->setVisible(true);
	});
	tgui::Button::Ptr gui_button = gui.get<tgui::Button>("GUI");
	gui_button->onClick([&gui] {
		auto settings_panel = gui.get<tgui::Group>("SettingsPanel");
		for (auto widget : settings_panel->getWidgets()) {
			widget->setVisible(false);
		}
		gui.get<tgui::Group>("GUIPanel")->setVisible(true);
	});
	tgui::Button::Ptr back = gui.get<tgui::Button>("Back");
	back->onClick([&gui, &close_groups] {
		close_groups(gui);
		gui.get<tgui::Group>("main_menu.txt")->setVisible(true);
	});
	// Initializing multiplayer menu
	init_multiplayer_menu("parameters.conf", gui);
}

void Menu_Processing::init(tgui::Gui &gui, Draw* draw_, b2Vec2* mouse_pos_,
	aux::Keyboard* keyboard_, Client_Network* network_,
	Game_Client* game_, Replay* replay_, bool* reload_) {
	game = game_;
	draw = draw_;
	keyboard = keyboard_;
	mouse_pos = mouse_pos_;
	network = network_;
	replay = replay_;
	reload = reload_;
	*reload = 1;
	_gui = &gui;
	init_tgui(gui);
	load_sound("sound_settings.conf", gui);
	load_HUD_settings("HUD_settings.conf", gui);
	// set main menu fields
	main_menu.set_draw(draw);
	main_menu.set_active(1);
	main_menu.set_events(&events);
	menus.push_back(&main_menu);
	init_menu("menu_configs/main.conf", &main_menu);
	// set client config menu fields
	config_menu.set_draw(draw);
	config_menu.set_active(0);
	config_menu.set_events(&events);
	std::string ServerIP, Port, ID, Name;
	load_config("client_config.conf", &ServerIP, &Port, &ID, &Name);
	menus.push_back(&config_menu);
	init_menu("menu_configs/client_config.conf", &config_menu);
	constant_texts[name_to_id["ServerIPText"]]->set_text("Server IP:");
	constant_texts[name_to_id["PortText"]]->set_text("Port:");
	constant_texts[name_to_id["IDText"]]->set_text("ID:");
	constant_texts[name_to_id["NameText"]]->set_text("Name:");
	text_fields[name_to_id["ServerIP"]]->set_text(ServerIP);
	text_fields[name_to_id["Port"]]->set_text(Port);
	constant_texts[name_to_id["ID"]]->set_text(ID);
	text_fields[name_to_id["Name"]]->set_text(Name);
	// set settigs menu 
	settings_menu.set_draw(draw);
	settings_menu.set_active(0);
	settings_menu.set_events(&events);
	menus.push_back(&settings_menu);
	init_menu("menu_configs/settings.conf", &settings_menu);
	// sound menu settings
	sound_menu.set_draw(draw);
	sound_menu.set_active(0);
	sound_menu.set_events(&events);
	menus.push_back(&sound_menu);
	init_menu("menu_configs/sound.conf", &sound_menu);
	constant_texts[name_to_id["SoundVolumeText"]]->set_text("Sound Volume:");
	constant_texts[name_to_id["MusicVolumeText"]]->set_text("Music Volume:");
	sliders[name_to_id["SoundVolume"]]->set_slider_value(game->get_audio()->get_sound_volume());
	sliders[name_to_id["MusicVolume"]]->set_slider_value(game->get_audio()->get_music_volume());
	// set keys menu fields
	keys_menu.set_draw(draw);
	keys_menu.set_active(0);
	keys_menu.set_events(&events);
	menus.push_back(&keys_menu);
	init_menu("menu_configs/keys.conf", &keys_menu);
	load_keys("keys.conf", &keys_menu_vec, gui, { 0, -350 }, -30, { 100, 50 }, 20);

	name_to_id["ApplySound"] = current_id++;
	name_to_id["Apply"] = current_id++;
	name_to_id["ApplySetup"] = current_id++;
	name_to_id["ApplyKeys"] = current_id++;
	name_to_id["ApplyClientConfig"] = current_id++;
	name_to_id["ApplyHUD"] = current_id++;
	name_to_id["ApplyReplay"] = current_id++;
	// set gun menu fields
	gun_menu.set_draw(draw);
	gun_menu.set_active(0);
	gun_menu.set_events(&events);
	menus.push_back(&gun_menu);
	init_gun_menu(b2Vec2(300, 100), "parameters.conf");
	// set hull menu fields
	hull_menu.set_draw(draw);
	hull_menu.set_active(0);
	hull_menu.set_events(&events);
	menus.push_back(&hull_menu);
	init_hull_menu(b2Vec2(300, 100), "parameters.conf");
	// set HUD menu fields
	HUD_menu.set_draw(draw);
	HUD_menu.set_active(0);
	HUD_menu.set_events(&events);
	init_menu("menu_configs/HUD.conf", &HUD_menu);
	constant_texts[name_to_id["AimConfigurationText"]]->set_text("Aim Configuration:");
	constant_texts[name_to_id["AimOpacityText"]]->set_text("Aim Opacity:");
	constant_texts[name_to_id["NetworkInformationText"]]->set_text("Network Information:");
	sliders[name_to_id["AimConfiguration"]]->set_slider_value(game->get_aim_conf());
	sliders[name_to_id["AimOpacity"]]->set_slider_value(game->get_aim_opacity());
	check_boxes[name_to_id["NetworkInformation"]]->set_state(game->get_network_information_active());
	menus.push_back(&HUD_menu);
	// set replay_menu
	replay_menu.set_draw(draw);
	replay_menu.set_active(0);
	replay_menu.set_events(&events);
	init_menu("menu_configs/replay.conf", &replay_menu);
	sliders[name_to_id["ReplaySlider"]]->set_slider_value(0);
	bars[name_to_id["ReplayBar"]]->set_value(0);
	bars[name_to_id["ReplayBar"]]->set_critical_value(-1);
	constant_texts[name_to_id["ReplayNameText"]]->set_text("Name:");
	constant_texts[name_to_id["ReplayIDText"]]->set_text("ID:");
	text_fields[name_to_id["ReplayName"]]->set_text(text_fields[name_to_id["Name"]]->get_text());
	text_fields[name_to_id["ReplayID"]]->set_text(constant_texts[name_to_id["ID"]]->get_text());
	constant_texts[name_to_id["ReplaySpeed"]]->set_text("Speed: " + aux::float_to_string(replay->get_replay_frame()->get_change_vel(), 0));
	menus.push_back(&replay_menu);
	// set replay_setup_menu
	replay_setup_menu.set_draw(draw);
	replay_setup_menu.set_active(0);
	replay_setup_menu.set_events(&events);
	init_menu("menu_configs/replay_setup.conf", &replay_setup_menu);
	constant_texts[name_to_id["ReplayPathText"]]->set_text("Replay path:");
	text_fields[name_to_id["ReplayPath"]]->set_text("example.ex");
	menus.push_back(&replay_setup_menu);
	// set modules menu
	modules_menu.set_draw(draw);
	modules_menu.set_active(0);
	modules_menu.set_events(&events);
	init_menu("menu_configs/modules.conf", &modules_menu);
	init_modules_menu(b2Vec2(300, 100), "parameters.conf");
	menus.push_back(&modules_menu);
}

void Menu_Processing::step() {
	replay->get_replay_frame()->step(1);
	if (replay->get_replay_active()) {
		int cur_time = replay->get_replay_frame()->get();
		_gui->get<tgui::Label>("CurTime")->setText(std::to_string(cur_time / 3600) + ":" +
			std::to_string((cur_time / 60) % 60) + ":" + std::to_string(cur_time % 60));
		_gui->get<tgui::Slider>("ReplaySlider")->setValue(cur_time);
	}
	if (active) {
		if (shader_active)
			game->get_draw()->fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(game->get_draw()->get_window()->getSize())),
				sf::Color(0, 0, 0, 90), 0);
		text_field_active = 0;
		sliders[name_to_id["ReplaySlider"]]->create(0, replay->get_replay_frame()->get_max());
		bars[name_to_id["ReplayBar"]]->set_max_value(replay->get_replay_frame()->get_max());
		bars[name_to_id["ReplayBar"]]->set_critical_value(-1);
		float foo = replay->get_replay_frame()->get_change_vel() * replay->get_replay_frame()->get_change_vel();
		if (foo < 1 - b2_epsilon && foo > b2_epsilon)
			constant_texts[name_to_id["ReplaySpeed"]]->set_text("Speed: " + aux::float_to_string(replay->get_replay_frame()->get_change_vel(), 1));
		else
			constant_texts[name_to_id["ReplaySpeed"]]->set_text("Speed: " + aux::float_to_string(replay->get_replay_frame()->get_change_vel(), 0));
		if (replay->get_replay_active())
			sliders[name_to_id["ReplaySlider"]]->set_slider_value(replay->get_replay_frame()->get());
		for (auto menu : menus) {
			menu->step();
			text_field_active |= menu->text_field_active;
		}
		for (auto keyboard_field : keyboard_fields) {
			auto it = id_to_keyit[keyboard_field.first];
			*keys_menu_vec[it.first][it.second] = keyboard_field.second->get_text();
		}
		game->set_aim_conf(sliders[name_to_id["AimConfiguration"]]->get_slider_value());
		game->set_aim_opacity(sliders[name_to_id["AimOpacity"]]->get_slider_value());
		game->set_network_information_active(check_boxes[name_to_id["NetworkInformation"]]->get_state());
		disactivated = 1;
	}
	if (!active) {
		if (disactivated) {
			if (!replay->get_replay_active())
				events.push(name_to_id["Apply"]);
			else
				events.push(name_to_id["ApplyReplay"]);
		}
		while (keyboard->text_entered->size())
			keyboard->text_entered->pop();
		disactivated = 0;
	}

	sf::Vector2f win_s =
		sf::Vector2f(1.0 * draw->get_window()->getSize().x / sf::VideoMode::getDesktopMode().width,
			1.0 * draw->get_window()->getSize().y / sf::VideoMode::getDesktopMode().height);
	auto av_rez = (win_s.x + win_s.y) / 2.0;
	tgui::Layout2d pic_size = tgui::Layout2d(96 * av_rez, 96 * av_rez);

	_gui->get<tgui::Picture>("CurrentHull")->setSize(pic_size);
	_gui->get<tgui::Picture>("CurrentHull")->setPosition(0, "5%");

	_gui->get<tgui::Picture>("CurrentGun")->setSize(pic_size);
	_gui->get<tgui::Picture>("CurrentGun")->setPosition((std::to_string(pic_size.x.getValue()) + " + 2%").c_str(), "5%");

	_gui->get<tgui::Picture>("CurrentLeftModule")->setSize(pic_size);
	_gui->get<tgui::Picture>("CurrentLeftModule")->setPosition((std::to_string(2 * pic_size.x.getValue()) + " + 4%").c_str(), "5%");

	_gui->get<tgui::Picture>("CurrentRightModule")->setSize(pic_size);
	_gui->get<tgui::Picture>("CurrentRightModule")->setPosition((std::to_string(3 * pic_size.x.getValue()) + " + 6%").c_str(), "5%");

	auto set_sizes = [=](tgui::ScrollablePanel::Ptr panel) {
		auto widgets = panel->getWidgets();
		for (int k = 0; k < widgets.size(); k++) {
			widgets[k]->setSize(pic_size);
			widgets[k]->setPosition((std::to_string((k % 4) * pic_size.x.getValue()) + " + " + std::to_string((k % 4) * 2) + "%").c_str(),
				(std::to_string((k / 4) * pic_size.x.getValue()) + " + " + std::to_string((k / 4) * 2) + "%").c_str());
		}
	};

	auto x = _gui->get<tgui::Group>("configuration.txt")->get<tgui::ScrollablePanel>("gun_vars");
	set_sizes(_gui->get<tgui::ScrollablePanel>("gun_vars"));
	set_sizes(_gui->get<tgui::ScrollablePanel>("hull_vars"));
	set_sizes(_gui->get<tgui::ScrollablePanel>("module_vars"));

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
			save_config("client_config.conf", text_fields[name_to_id["ServerIP"]]->get_text(), atoi(text_fields[name_to_id["Port"]]->get_text().c_str()),
				atoi(constant_texts[name_to_id["ID"]]->get_text().c_str()), text_fields[name_to_id["Name"]]->get_text());
			network->set_server(text_fields[name_to_id["ServerIP"]]->get_text());
			network->set_port(atoi(text_fields[name_to_id["Port"]]->get_text().c_str()));
			network->set_id(atoi(constant_texts[name_to_id["ID"]]->get_text().c_str()));
			network->set_name(text_fields[name_to_id["Name"]]->get_text());
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
			save_HUD_settings("HUD_settings.conf");
		}
		if (name_to_id["HUD"] == events.front()) {
			close_settings_menus();
			HUD_menu.set_active(1);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["Modules"] == events.front()) {
			if (module_num == 1) {
				events.push(name_to_id[game->get_left_module_name() + "-module"]);
			}
			else {
				events.push(name_to_id[game->get_right_module_name() + "-module"]);
			}
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["PlayReplay"] == events.front()) {
			shader_active = 0;
			replay_setup_menu.set_active(0);
			replay->set_replay_active(1);
			replay->set_replay_path("replays/" + text_fields[name_to_id["ReplayPath"]]->get_text());
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
		for (auto it = modules.begin(); it != modules.end(); it++) {
			std::string cur_name = it->first.substr(0, it->first.size() - 1);
			if (name_to_id[cur_name + "-module"] == events.front()) {
				if (module_num == 1)
					game->set_left_module_name(cur_name);
				else
					game->set_right_module_name(cur_name);
				close_settings_menus();
				modules_menu.set_active(1);
				modules[game->get_left_module_name() + "1"].set_active(1);
				modules[game->get_right_module_name() + "2"].set_active(1);
				events.push(name_to_id["Apply"]);
			}
			if (name_to_id[cur_name + "1"] == events.front()) {
				module_num = 1;
			}
			if (name_to_id[cur_name + "2"] == events.front()) {
				module_num = 2;
			}
			it++;
		}
		if (name_to_id["ApplySetup"] == events.front()) {
			//set_current_gun("setup.conf", cur_gun);
			game->save_setup("setup.conf");
		}
		if (name_to_id["ReplayBack"] == events.front()) {
			shader_active = 1;
			replay_setup_menu.set_active(1);
			replay->set_replay_active(0);
			replay_menu.set_active(0);
			events.push(name_to_id["Apply"]);
		}
		if (name_to_id["ReplaySetupBack"] == events.front()) {
			replay_setup_menu.set_active(0);
			main_menu.set_active(1);
		}
		if (name_to_id["Replay"] == events.front()) {
			replay_setup_menu.set_active(1);
			main_menu.set_active(0);
		}
		if (name_to_id["ApplyReplay"] == events.front()) {
			network->set_name(text_fields[name_to_id["ReplayName"]]->get_text());
			network->set_id(atoi(text_fields[name_to_id["ReplayID"]]->get_text().c_str()));
		}
		events.pop();
	}
}
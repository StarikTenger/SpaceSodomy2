#include "MenuProcessing.h"

MenuProcessing::MenuProcessing() {}

void MenuProcessing::save_keys(std::string path, std::vector<std::vector<std::string*>> keys) {
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
void MenuProcessing::load_keys(std::string path, std::vector<std::vector<std::string*>>* keys, tgui::Gui& gui) {
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

void MenuProcessing::load_HUD_settings(std::string path, tgui::Gui &gui) {
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

void MenuProcessing::save_HUD_settings(std::string path) {
	std::ofstream fout;
	fout.open(path);
	fout << game->get_aim_conf() << "\n" << game->get_aim_opacity() << "\n" << game->get_network_information_active();
	fout.close();
}

void MenuProcessing::load_sound(std::string path, tgui::Gui &gui) {
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

void MenuProcessing::save_sound(std::string path) {
	std::ofstream fout;
	fout.open(path);
	fout << game->get_audio()->get_sound_volume() << "\n" << game->get_audio()->get_music_volume();
	fout.close();
}

void MenuProcessing::save_config(std::string path, std::string address_, int port_, int id_, std::string name_) {
	std::ofstream fout;
	fout.open(path);
	fout << address_ << " " << port_ << " " << id_ << " " << name_;
	fout.close();
}
void MenuProcessing::load_config(std::string path, std::string* address_, std::string* port_,
	std::string* id_, std::string* name_, tgui::Gui &gui) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	config >> *(address_);
	config >> *(port_);
	config >> *(id_);
	config >> *(name_);
	gui.get<tgui::EditBox>("ServerIP")->setText(*address_);
	gui.get<tgui::EditBox>("Name")->setText(*name_);
	if ((*id_) == "0") {
		*id_ = std::to_string(aux::random_int(1, 100000));
		save_config(path, *address_, atoi(port_->c_str()), atoi(id_->c_str()), *name_);
	}
}

void MenuProcessing::toggle_active() {
	active = !active;
	auto wid = _gui->getWidgets();
	for (int i = 0; i < wid.size(); i++) {
		wid[i]->setVisible(false);
	}
	_gui->get<tgui::Group>("HUD.txt")->setVisible(!active);
	_gui->get<tgui::Group>("replay.txt")->setVisible(replay->get_replay_active() && active);
	_gui->get<tgui::Group>("main_menu.txt")->setVisible(!replay->get_replay_active() && active);
	text_field_active = active;
}

void MenuProcessing::init_multiplayer_menu(std::string file_name, tgui::Gui& gui) {
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
		module_num = 1;
	});

	conf->get<tgui::Picture>("CurrentRightModule")->getRenderer()->setTexture(*draw->get_texture(game->get_right_module_name() + "-module"));
	conf->get<tgui::Picture>("CurrentRightModule")->onClick([=] {
		gun_vars->setVisible(0);
		hull_vars->setVisible(0);
		module_vars->setVisible(1);
		module_num = 2;
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
				auto texture = new_gun->get<tgui::Picture>("GunPreview")->getRenderer()->getTexture();
				gui.get<tgui::Picture>("CurrentGun")->getRenderer()->setTexture(texture);
				gui.get<tgui::Picture>("GunImage")->getRenderer()->setTexture(texture);
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
				else {
					rval = val;
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
				auto texture = new_hull->get<tgui::Picture>("HullPreview")->getRenderer()->getTexture();
				gui.get<tgui::Picture>("CurrentHull")->getRenderer()->setTexture(texture);
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
				else {
					rval = val;
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
					auto texture = new_module->get<tgui::Picture>("ModulePreview")->getRenderer()->getTexture();
					gui.get<tgui::Picture>("CurrentLeftModule")->getRenderer()->setTexture(texture);
					gui.get<tgui::Picture>("LeftModule")->getRenderer()->setTexture(texture);
				}
				else {
					_game->set_right_module_name(new_module->get<tgui::Label>("ModuleName")->getText().toStdString());
					auto texture = new_module->get<tgui::Picture>("ModulePreview")->getRenderer()->getTexture();
					gui.get<tgui::Picture>("CurrentRightModule")->getRenderer()->setTexture(texture);
					gui.get<tgui::Picture>("RightModule")->getRenderer()->setTexture(texture);
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
				else {
					rval = val;
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
			auto bar = gun->get<tgui::ProgressBar>(param.first);
			bar->setMaximum(param.second);
			bar->setText(bar->getValue() + "/" + bar->getMaximum());
		}
	}

	for (auto hull : hulls) {
		for (auto param : hull_maxes) {
			auto bar = hull->get<tgui::ProgressBar>(param.first);
			bar->setMaximum(param.second);
			bar->setText(bar->getValue() + "/" + bar->getMaximum());
		}
	}

	for (auto module : modules) {
		for (auto param : module_maxes) {
			auto bar = module->get<tgui::ProgressBar>(param.first);
			bar->setMaximum(param.second);
			bar->setText(bar->getValue() + "/" + bar->getMaximum());
		}
	}
}

void MenuProcessing::init_tgui(tgui::Gui& gui) {
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
	auto launch_replay = [=](tgui::Gui& gui, std::string name, Replay* replay) {
		replay->set_replay_path("replays/" + name);
		replay->set_replay_active(1);
		gui.get<tgui::Label>("ReplayName")->setText(name);
		int max_time = replay->get_replay_frame()->get_max();
		std::cout << max_time << " jodfshiedu\n";
		gui.get<tgui::Label>("MaxTime")->setText(std::to_string(max_time / 3600) + ":" +
			std::to_string((max_time / 60) % 60) + ":" + std::to_string(max_time % 60));
		gui.get<tgui::Slider>("ReplaySlider")->setMaximum(replay->get_frame_number());
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
		launch_replay(gui, "example.ex", replay);
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
		auto _replay = replay;
		choose_file->onFileSelect([=, &gui] {
			auto name = choose_file->getSelectedPaths()[0].getFilename().toStdString();
			launch_replay(gui, name, _replay);
			});
			gui.get<tgui::Group>("replay.txt")->add(choose_file);
	});
	auto spin_control = gui.get<tgui::SpinControl>("SpinControl");
	spin_control->onValueChange([=](float val) {
		replay->set_speed(val);
	});
	auto replay_slider = gui.get<tgui::Slider>("ReplaySlider");
	replay_slider->onValueChange([=](int val) {
		replay->set_frame(val);
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
	gui.get<tgui::EditBox>("ServerIP")->onTextChange([=, &gui] {
		network->set_server(gui.get<tgui::EditBox>("ServerIP")->getText().toStdString());
		save_config("client_config.conf", network->get_serverIP(), network->get_port(), network->get_id(), network->get_name());
	});
	gui.get<tgui::EditBox>("Name")->onTextChange([=, &gui] {
		network->set_name(gui.get<tgui::EditBox>("Name")->getText().toStdString());
		save_config("client_config.conf", network->get_serverIP(), network->get_port(), network->get_id(), network->get_name());
	});
	gui.get<tgui::Button>("Play")->onClick([=] {
		toggle_active();
	});
}

void MenuProcessing::init(tgui::Gui &gui, Draw* draw_, b2Vec2* mouse_pos_,
	aux::Keyboard* keyboard_, ClientNetwork* network_,
	GameClient* game_, Replay* replay_, bool* reload_) {
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
	std::string ServerIP, Port, ID, Name;
	load_config("client_config.conf", &ServerIP, &Port, &ID, &Name, gui);
	load_keys("keys.conf", &keys_menu_vec, gui);
}

void MenuProcessing::step() {
	replay->step(game->get_dt());
	
	if (replay->get_replay_active()) {
		int cur_time = replay->get_seconds();
		_gui->get<tgui::Label>("CurTime")->setText(std::to_string(cur_time / 3600) + ":" +
			std::to_string((cur_time / 60) % 60) + ":" + std::to_string(cur_time % 60));
		_gui->get<tgui::Slider>("ReplaySlider")->setValue(replay->get_cur_frame_number());
	}
	if (slider_changing != _gui->get<tgui::Slider>("ReplaySlider")->isMouseDown()) {
		slider_changing = !slider_changing;
		replay->play_button(*_gui);
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
		if (name_to_id["ApplyKeys"] == events.front()) { // Apply button
			save_keys("keys.conf", keys_menu_vec);
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
		if (name_to_id["ApplyHUD"] == events.front()) {
			save_HUD_settings("HUD_settings.conf");
		}
		if (name_to_id["ApplySetup"] == events.front()) {
			//set_current_gun("setup.conf", cur_gun);
			game->save_setup("setup.conf");
		}
		events.pop();
	}
}
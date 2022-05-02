#include "MenuProcessing.h"
MenuProcessing::MenuProcessing() {}

void MenuProcessing::save_keys(std::string path, 
	std::vector<std::vector<std::string*>> keys) {
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

// init controls page in settings 
void MenuProcessing::load_keys(std::string path,
	std::vector<std::vector<std::string*>>* keys) {

	int k = 0;
	auto controls_panel = _gui->get<tgui::ScrollablePanel>("ControlsPanel");

	// scrollbar
	auto scrollbar = tgui::ScrollbarRenderer(controls_panel->getRenderer()->getScrollbar());
	scrollbar.setTrackColor("None");
	scrollbar.setThumbColor("#448ACC");
	scrollbar.setThumbColorHover("None");
	scrollbar.setArrowColor("#448ACC");
	scrollbar.setArrowBackgroundColor("None");
	scrollbar.setArrowBackgroundColorHover("None");
		
	std::ifstream file_to_comment(path);	
	std::stringstream config = aux::comment(file_to_comment);

	// read keybinds from config
	for (int i = 0; !config.eof(); i++) {
		if (keys->size() == i)
			keys->push_back(std::vector<std::string*>());
		std::string cur, cur_name;
		config >> cur_name >> cur;
		for (int j = 0; !config.eof() && (cur != "END"); j++) {
			if (j == 0) {

				//name of the action
				auto label = tgui::Label::create();
				label->setText(cur_name);
				label->getRenderer()->setTextColor("#448ACC");
				auto size = tgui::Layout2d(
					tgui::Layout("50%"),   //size of action name
					tgui::Layout("7%")
				);
				label->setSize(size);
				auto layout = tgui::Layout2d(
					tgui::Layout(std::to_string(5) + "%"),    //position of action name
					tgui::Layout(std::to_string(7 * i)+"%")
				);
				label->setPosition(layout);
				controls_panel->add(label);
			}

			if (j == keys->operator[](i).size()) 
				keys->operator[](i).push_back(new std::string(cur));
			else
				*(keys->operator[](i)[j]) = cur;

			//keybinding box
			auto keybinding = KeybindingBox::create();

			//keybinding box visuals
			keybinding->getRenderer()->setTexture("../textures/menu/ButtonMid.png");
			keybinding->getRenderer()->setTextureHover("../textures/menu/ButtonMidHover.png");
			keybinding->getRenderer()->setTextureFocused("../textures/menu/ButtonMidFocused.png");
			keybinding->getRenderer()->setTextColor("#448ACC");
			keybinding->getRenderer()->setTextColorFocused("#4BCC3D");
			keybinding->getRenderer()->setCaretColor("None");
			keybinding->setAlignment(tgui::EditBox::Alignment::Center);


			keybinding->setText(cur);
			auto size = tgui::Layout2d(
				tgui::Layout("20%"),   //size of keybinding window
				tgui::Layout("7%")
			);
			keybinding->setSize(size);
			auto layout = tgui::Layout2d(
				tgui::Layout(std::to_string(55 + j * 20) + "%"),    //position of keybinding window
				tgui::Layout(std::to_string(7 * i) + "%")
			);
			keybinding->setPosition(layout);
			controls_panel->add(keybinding);
			std::string name = "keybinding" + std::to_string(k);
			keybinding->setWidgetName(name);
			keybinding->keys = keys;
			keybinding->reload = reload;
			k++;

			//key rebind
			keybinding->onKeyPressed([=]() {
				auto key = _gui->get<KeybindingBox>(name)->keys->operator[](i)[j];
				auto new_key = _gui->get<KeybindingBox>(name)->getText().toStdString();
				*key = new_key;
				auto _reload = _gui->get<KeybindingBox>(name)->reload;
				*_reload = true;
			});
			config >> cur;
		}
	}
}

void MenuProcessing::load_HUD_settings(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	double next;
	config >> next;
	game->set_aim_conf(next);
	_gui->get<tgui::Slider>("AimConfiguration")->setValue(next);
	_gui->get<tgui::Slider>("AimConfiguration")->onValueChange([=](int val) {
		game->set_aim_conf(val);
		save_HUD_settings("HUD_settings.conf");
	});
	config >> next;
	game->set_aim_opacity(next);
	_gui->get<tgui::Slider>("AimOpacity")->setValue(next);
	_gui->get<tgui::Slider>("AimOpacity")->onValueChange([=](int val) {
		game->set_aim_opacity(val);
		save_HUD_settings("HUD_settings.conf");
	});
	config >> next;
	game->set_network_information_active(next);
	_gui->get<tgui::CheckBox>("NetworkInformation")->setChecked(next);
	_gui->get<tgui::CheckBox>("NetworkInformation")->onChange([=](bool val) {
		game->set_network_information_active(val);
		save_HUD_settings("HUD_settings.conf");
	});
}

void MenuProcessing::save_HUD_settings(std::string path) {
	std::ofstream fout;
	fout.open(path);
	fout << game->get_aim_conf() << "\n" << game->get_aim_opacity() << "\n" << game->get_network_information_active();
	fout.close();
}

void MenuProcessing::load_sound(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	auto music_slider = _gui->get<tgui::Slider>("MusicSlider");
	auto sound_slider = _gui->get<tgui::Slider>("SoundSlider");
	double volume;
	config >> volume;
	game->get_audio()->set_sound_volume(volume);
	sound_slider->setValue(volume);
	sound_slider->onValueChange([=](int new_val) {
		game->get_audio()->set_sound_volume(new_val);
		save_sound("sound_settings.conf");
	});
	config >> volume;
	game->get_audio()->set_music_volume(volume);
	music_slider->setValue(volume);
	music_slider->onValueChange([=](int new_val) {
		game->get_audio()->set_music_volume(new_val);
		save_sound("sound_settings.conf");
	});
}

void MenuProcessing::save_sound(std::string path) {
	std::ofstream fout;
	fout.open(path);
	fout << game->get_audio()->get_sound_volume() << "\n" << game->get_audio()->get_music_volume();
	fout.close();
}

void MenuProcessing::save_config(std::string path, 
	std::string address_, int port_, int id_, std::string name_) {
	std::ofstream fout;
	fout.open(path);
	fout << address_ << " " << port_ << " " << id_ << " " << name_;
	fout.close();
}

void MenuProcessing::load_config(std::string path, std::string* address_, 
	std::string* port_, std::string* id_, std::string* name_) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	config >> *(address_);
	config >> *(port_);
	config >> *(id_);
	config >> *(name_);
	_gui->get<tgui::EditBox>("ServerIP")->setText(*address_);
	_gui->get<tgui::EditBox>("Name")->setText(*name_);
	if ((*id_) == "0") {
		*id_ = std::to_string(aux::random_int(1, 100000));
		save_config(path, *address_, atoi(port_->c_str()), atoi(id_->c_str()), *name_);
	}
}

void MenuProcessing::close_widgets(tgui::Container::Ptr container) {
	auto widgets = container->getWidgets();
	for (auto widget : widgets)
		widget->setVisible(false);
}

void MenuProcessing::close_groups() {
	auto widgets = _gui->getWidgets();
	for (auto widget : widgets)
		widget->setVisible(false);
};

void MenuProcessing::toggle_active() {
	active = !active;
	close_groups();
	_gui->get<tgui::Group>("HUD.txt")->setVisible(!active);
	_gui->get<tgui::Group>("replay.txt")->setVisible(replay->get_replay_active() && active);
	_gui->get<tgui::Group>("main_menu.txt")->setVisible(!replay->get_replay_active() && active);
	text_field_active = active;
}

void MenuProcessing::init_multiplayer_menu(std::string file_name) {
	auto conf = _gui->get<tgui::Group>("configuration.txt");

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
	auto close_info = [this]() {
		auto infogroup = _gui->get<tgui::Group>("InfoGroup");
		close_widgets(infogroup);
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
			pic->onClick([=] {
				close_info();
				new_gun->setVisible(1);

				_game->set_gun_name(new_gun->get<tgui::Label>("GunName")->getText().toStdString());
				auto texture = new_gun->get<tgui::Picture>("GunPreview")->getRenderer()->getTexture();
				_gui->get<tgui::Picture>("CurrentGun")->getRenderer()->setTexture(texture);
				game->save_setup("setup.conf");
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
			_gui->get<tgui::Group>("InfoGroup")->add(new_gun);
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
			pic->onClick([=, &close_info] {
				close_info();
				new_hull->setVisible(1);

				_game->set_hull_name(new_hull->get<tgui::Label>("HullName")->getText().toStdString());
				auto texture = new_hull->get<tgui::Picture>("HullPreview")->getRenderer()->getTexture();
				_gui->get<tgui::Picture>("CurrentHull")->getRenderer()->setTexture(texture);
				game->save_setup("setup.conf");
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
			_gui->get<tgui::Group>("InfoGroup")->add(new_hull);
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
			pic->onClick([=, &close_info] {
				close_info();
				new_module->setVisible(1);

				if (module_num == 1) {
					_game->set_left_module_name(new_module->get<tgui::Label>("ModuleName")->getText().toStdString());
					auto texture = new_module->get<tgui::Picture>("ModulePreview")->getRenderer()->getTexture();
					_gui->get<tgui::Picture>("CurrentLeftModule")->getRenderer()->setTexture(texture);
				}
				else {
					_game->set_right_module_name(new_module->get<tgui::Label>("ModuleName")->getText().toStdString());
					auto texture = new_module->get<tgui::Picture>("ModulePreview")->getRenderer()->getTexture();
					_gui->get<tgui::Picture>("CurrentRightModule")->getRenderer()->setTexture(texture);
				}
				game->save_setup("setup.conf");
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
			_gui->get<tgui::Group>("InfoGroup")->add(new_module);
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

void MenuProcessing::open_replay_menu(){
	auto launch_replay = [=](std::string name, Replay* replay) {
		replay->set_replay_path("replays/" + name);
		replay->set_replay_active(1);
		_gui->get<tgui::Label>("ReplayName")->setText(name);
		int max_time = replay->get_replay_frame()->get_max();
		std::cout << max_time << " jodfshiedu\n";
		_gui->get<tgui::Label>("MaxTime")->setText(std::to_string(max_time / 3600) + ":" +
			std::to_string((max_time / 60) % 60) + ":" + std::to_string(max_time % 60));
		_gui->get<tgui::Slider>("ReplaySlider")->setMaximum(replay->get_frame_number());
	};

	// Open replay menu
	tgui::Button::Ptr replayButton = _gui->get<tgui::Button>("Replay");
	replayButton->onClick([=] {
		close_groups();
		launch_replay("example.ex", replay);
		_gui->get<tgui::Group>("replay.txt")->setVisible(true);
	});

	// Initializing replay menu
	auto select_replay_button = _gui->get<tgui::Button>("SelectReplay");
	select_replay_button->onClick([=, &launch_replay] {
		auto choose_file = tgui::FileDialog::create("Open replay", "Open");
		auto _replay = replay;
		choose_file->onFileSelect([=] {
			auto name = choose_file->getSelectedPaths()[0].getFilename().toStdString();
			launch_replay(name, _replay);
			});
		_gui->get<tgui::Group>("replay.txt")->add(choose_file);
	});
	auto spin_control = _gui->get<tgui::SpinControl>("SpinControl");
	spin_control->onValueChange([=](float val) {
		replay->set_speed(val);
	});
	auto replay_slider = _gui->get<tgui::Slider>("ReplaySlider");
	replay_slider->onValueChange([=](int val) {
		replay->set_frame(val);
	});
	auto replay_play_button = _gui->get<tgui::Button>("ReplayPlay");
	replay_play_button->onPress([=] {
		replay->play_button(*_gui);
	});
}

// Draws green selction box around settings buttons
void MenuProcessing::reset_settings_textures() {
	auto settings_panel = _gui->get<tgui::Group>("SettingsPanel");
	auto controls_panel = _gui->get<tgui::ScrollablePanel>("ControlsPanel");
	auto audio_panel = _gui->get<tgui::Group>("AudioPanel");
	auto hud_panel = _gui->get<tgui::Group>("HUDPanel");

	auto audio_button = _gui->get<tgui::ButtonBase>("Audio");
	auto controls_button = _gui->get<tgui::ButtonBase>("Controls");
	auto hud_button = _gui->get<tgui::ButtonBase>("HUD");

	// Setting default texture
	audio_button->getRenderer()->setTexture("../textures/menu/Buttontop.png");
	controls_button->getRenderer()->setTexture("../textures/menu/Buttonmid.png");
	hud_button->getRenderer()->setTexture("../textures/menu/Buttonmid.png");
	audio_button->getRenderer()->setTextColor("#448ACC");
	controls_button->getRenderer()->setTextColor("#448ACC");
	hud_button->getRenderer()->setTextColor("#448ACC");

	// Choosing the right one
	for (auto widget : settings_panel->getWidgets()) {
		if (widget->isVisible()) {
			if (widget == audio_panel) {
				audio_button->getRenderer()->setTexture("../textures/menu/ButtonTopFocused.png");
				audio_button->getRenderer()->setTextColor("#4BCC3D");
			}
			else if (widget == controls_panel) {
				controls_button->getRenderer()->setTexture("../textures/menu/ButtonMidFocused.png");
				controls_button->getRenderer()->setTextColor("#4BCC3D");
			}
			else if (widget == hud_panel) {
				hud_button->getRenderer()->setTexture("../textures/menu/ButtonMidFocused.png");
				hud_button->getRenderer()->setTextColor("#4BCC3D");
			}
		}
	}
}

void MenuProcessing::open_settings_menu() {
	// Open settings menu
	tgui::Button::Ptr settings = _gui->get<tgui::Button>("Settings");

	settings->onClick([=] {
		close_groups();
		reset_settings_textures();
		_gui->get<tgui::Group>("settings.txt")->setVisible(true);
	});

	// Open controls in settings menu
	tgui::Button::Ptr controls = _gui->get<tgui::Button>("Controls");
	controls->onClick([this] {
		auto settings_panel = _gui->get<tgui::Group>("SettingsPanel");
		close_widgets(settings_panel);
		_gui->get<tgui::Group>("ControlsPanel")->setVisible(true);
		reset_settings_textures();
	});

	// Open audio in settings menu
	tgui::Button::Ptr audio = _gui->get<tgui::Button>("Audio");
	audio->onClick([this] {
		auto settings_panel = _gui->get<tgui::Group>("SettingsPanel");
		close_widgets(settings_panel);
		_gui->get<tgui::Group>("AudioPanel")->setVisible(true);
		reset_settings_textures();
	});

	// Open hud in settings menu
	tgui::Button::Ptr hud_button = _gui->get<tgui::Button>("HUD");
	hud_button->onClick([this] {
		auto settings_panel = _gui->get<tgui::Group>("SettingsPanel");
		close_widgets(settings_panel);
		_gui->get<tgui::Group>("HUDPanel")->setVisible(true);
		reset_settings_textures();
	});

	// Back-button in settings menu
	tgui::Button::Ptr back = _gui->get<tgui::Button>("Back");
	back->onClick([this] {
		close_groups();
		_gui->get<tgui::Group>("main_menu.txt")->setVisible(true);
	});
}

void MenuProcessing::open_multiplayer_menu() {
	// Open multiplayer menu
	auto multiplayer = _gui->get<tgui::Button>("Multiplayer");
	multiplayer->onClick([=] {
		close_groups();
		_gui->get<tgui::Group>("configuration.txt")->setVisible(true);
	});


	// Initializing multiplayer menu
	init_multiplayer_menu("parameters.conf");
	_gui->get<tgui::EditBox>("ServerIP")->onTextChange([=] {
		network->set_server(_gui->get<tgui::EditBox>("ServerIP")->getText().toStdString());
		save_config("client_config.conf", network->get_serverIP(), network->get_port(), network->get_id(), network->get_name());
	});
	_gui->get<tgui::EditBox>("Name")->onTextChange([=] {
		network->set_name(_gui->get<tgui::EditBox>("Name")->getText().toStdString());
		save_config("client_config.conf", network->get_serverIP(), network->get_port(), network->get_id(), network->get_name());
	});
	_gui->get<tgui::Button>("Play")->onClick([=] {
		toggle_active();
	});
}

void MenuProcessing::open_exit_menu(){
	// exit
	auto exit = _gui->get<tgui::Button>("Exit");
	exit->onClick([=] {
		draw->get_window()->close();
	});
}

void MenuProcessing::init_tgui() {
	auto load_widgets = [this](std::string file_name) {
		auto ans = tgui::Group::create();
		ans->loadWidgetsFromFile(file_name);
		ans->setVisible(false);
		_gui->add(ans);
		ans->setWidgetName(file_name);
		return ans;
	};
	auto main_menu = load_widgets("main_menu.txt");
	main_menu->setVisible(true);
	auto HUD = load_widgets("HUD.txt");
	auto settings_menu = load_widgets("settings.txt");
	auto configuration_menu = load_widgets("configuration.txt");
	auto replay_menu = load_widgets("replay.txt");
	open_replay_menu();
	open_settings_menu();
	open_multiplayer_menu();
	open_exit_menu();
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
	init_tgui();
	load_sound("sound_settings.conf");
	load_HUD_settings("HUD_settings.conf");
	std::string ServerIP, Port, ID, Name;
	load_config("client_config.conf", &ServerIP, &Port, &ID, &Name);
	load_keys("keys.conf", &keys_menu_vec);
}

void MenuProcessing::widget_formating(std::string groupname, 
	float aspectratio, float textscale) {
	//saves aspect ratio of the group and updates text size
	auto widget = _gui->get <tgui::Widget>(groupname);
	if (widget->isVisible()) {
		float windowHeight = widget->getSize().y;
		widget->setSize({ windowHeight * aspectratio }, "100%");
		widget->setTextSize(windowHeight * textscale);
	}
}

void MenuProcessing::scrollbar_formating(std::string scrollablepanelname, float scale) {
	auto scrollable_panel = _gui->get<tgui::ScrollablePanel>(scrollablepanelname);
	if (scrollable_panel->isVisible()) {
		scrollable_panel->getRenderer()->setScrollbarWidth(scrollable_panel->getSize().x * scale);
	}
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

	widget_formating("main_menu.txt", 1.3, 0.03);
	widget_formating("settings.txt", 1.3, 0.025);
	scrollbar_formating("ControlsPanel", 0.01);
}
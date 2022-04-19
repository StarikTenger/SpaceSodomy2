#include "Control.h"

int Control::key_by_name(std::string name) {
	int sum = 0;
	for (auto k : key_matches[name])
		sum += keyboard.state_current[k];
	return sum;
}

int Control::key_prev_by_name(std::string name) {
	int sum = 0;
	for (auto k : key_matches[name])
		sum += keyboard.state_prev[k];
	return sum;

}

void Control::process_events(sf::Window* window) {
	window->setKeyRepeatEnabled(true);
	sf::Event event;

	while (window->pollEvent(event)) {
		gui.handleEvent(event);
		switch (event.type) {
		case sf::Event::Closed:
			is_running = 1;
			window->close();
			break;
		case sf::Event::MouseMoved:
			mouse_pos.x = event.mouseMove.x;
			mouse_pos.y = event.mouseMove.y;
			break;
		case sf::Event::TextEntered:
			
			text_entered.push(wchar_t(event.text.unicode));
			break;
		}
	}

	window->setKeyRepeatEnabled(false);

	// Processing keyboard
	keyboard.state_prev = keyboard.state_current;
	for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
		keyboard.state_current[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
	}
}

void Control::process_commands() {
	command_module.reset();

	bool commands_active = !menu_processing.text_field_active;

	if (!draw.get_window()->hasFocus())
		return;

	// TODO: do it in one std::map
	if (commands_active) {
		if (key_by_name("ENGINE_LIN_FORWARD")) 
			command_module.set_command(CommandModule::ENGINE_LIN_FORWARD, 1);
		if (key_by_name("ENGINE_LIN_BACKWARD"))
			command_module.set_command(CommandModule::ENGINE_LIN_BACKWARD, 1);
		if (key_by_name("ENGINE_LIN_LEFT"))
			command_module.set_command(CommandModule::ENGINE_LIN_LEFT, 1);
		if (key_by_name("ENGINE_LIN_RIGHT"))
			command_module.set_command(CommandModule::ENGINE_LIN_RIGHT, 1);
		if (key_by_name("ENGINE_ANG_LEFT"))
			command_module.set_command(CommandModule::ENGINE_ANG_LEFT, 1);
		if (key_by_name("ENGINE_ANG_RIGHT"))
			command_module.set_command(CommandModule::ENGINE_ANG_RIGHT, 1);
		if (key_by_name("STABILIZE_ROTATION") || stabilize_rotation_auto)
			command_module.set_command(CommandModule::STABILIZE_ROTATION, 1);
		if (key_by_name("STABILIZE_ROTATION_AUTO") && !key_prev_by_name("STABILIZE_ROTATION_AUTO"))
			stabilize_rotation_auto = !stabilize_rotation_auto;
		if (key_by_name("SHOOT"))
			command_module.set_command(CommandModule::SHOOT, 1);
		if (key_by_name("BOOST"))
			command_module.set_command(CommandModule::BOOST, 1);
		if (key_by_name("RESPAWN"))
			command_module.set_command(CommandModule::RESPAWN, 1);
		if (key_by_name("BONUS_ACTIVATION"))
			command_module.set_command(CommandModule::BONUS_ACTIVATION, 1);
		if (key_by_name("LEFT_MODULE"))
			command_module.set_command(CommandModule::LEFT_MODULE, 1);
		if (key_by_name("RIGHT_MODULE"))
			command_module.set_command(CommandModule::RIGHT_MODULE, 1);
	}
	if (key_by_name("FULLSCREEN"))
		draw.fullscreen_toggle();

	if (key_by_name("REPLAY_SPEED_UP") && replay.get_replay_active() && !key_prev_by_name("REPLAY_SPEED_UP")) {
			auto spin = gui.get<tgui::SpinControl>("SpinControl");
			spin->setValue(spin->getValue() + spin->getStep());
		}
		if (key_by_name("REPLAY_SPEED_DOWN") && replay.get_replay_active() && !key_prev_by_name("REPLAY_SPEED_DOWN")) {
			auto spin = gui.get<tgui::SpinControl>("SpinControl");
			spin->setValue(spin->getValue() - spin->getStep());
		}
		if (key_by_name("NEXT_PLAYER") && replay.get_replay_active() && !key_prev_by_name("NEXT_PLAYER")) {
			if (game.get_players()->operator[](network.get_id()) == nullptr) {
				game.get_players()->erase(game.get_players()->find(network.get_id()));
				if (game.get_players()->size() != 0) {
					network.set_id(game.get_players()->begin()->second->get_id());
					gui.get<tgui::Label>("PlayerName")->setText(game.get_players()->begin()->second->get_name());
				}
			} else {
				for (auto player = game.get_players()->begin(); player != game.get_players()->end(); player++) {
					if (player->second->get_id() == network.get_id()) {
						player++;
						if (player == game.get_players()->end()) {
							network.set_id(0);
							gui.get<tgui::Label>("PlayerName")->setText("None");
						} else {
							network.set_id(player->second->get_id());
							gui.get<tgui::Label>("PlayerName")->setText(player->second->get_name());
						}
						break;
					}
				}
			}
		}
		if (key_by_name("PREVIOUS_PLAYER") && replay.get_replay_active() && !key_prev_by_name("PREVIOUS_PLAYER")) {
			if (game.get_players()->operator[](network.get_id()) == nullptr) {
				game.get_players()->erase(game.get_players()->find(network.get_id()));
				if (game.get_players()->size() != 0) {
					auto player = game.get_players()->end();
					player--;
					network.set_id(player->second->get_id());
					gui.get<tgui::Label>("PlayerName")->setText(player->second->get_name());
				}
			}
			else {
				for (auto player = game.get_players()->begin(); player != game.get_players()->end(); player++) {
					if (player->second->get_id() == network.get_id()) {
						if (player == game.get_players()->begin()) {
							network.set_id(0);
							gui.get<tgui::Label>("PlayerName")->setText("None");
						}
						else {
							player--;
							network.set_id(player->second->get_id());
							gui.get<tgui::Label>("PlayerName")->setText(player->second->get_name());
						}
						break;
					}
				}
			}
		}
		if (key_by_name("REPLAY_PLAY") && replay.get_replay_active() && !key_prev_by_name("REPLAY_PLAY")) {
			replay.play_button(gui);
		}

	// Zoom out
	if (commands_active) {
		if (key_by_name("ZOOM_OUT"))
			draw.get_camera()->modify_scale(pow(2, -delay * 0.001 * zoom_vel));
		if (key_by_name("ZOOM_IN"))
			draw.get_camera()->modify_scale(pow(2, delay * 0.001 * zoom_vel));
	}

	// Menu
	if (key_by_name("MENU") && !key_prev_by_name("MENU")) {
		menu_processing.toggle_active();
	}
}

std::string Control::commands_to_string() {
	std::string message = "";
	message += game.get_gun_name() + " ";
	message += game.get_hull_name() + " ";
	message += game.get_left_module_name() + " ";
	message += game.get_right_module_name() + " ";
	message += "#";
	for (int i = 0; i < CommandModule::COMMAND_COUNT; i++) {
		if (command_module.get_command(i))
			message += "1";
		else
			message += "0";
	}
	return message;
}

Control::Control() {
	network.set_id(1);
	draw.create_window(600, 600, "Space Sodomy II");
	// GUI implemetation
	// *****************
	gui.setWindow(*draw.get_window());
	tgui::setResourcePath("gui_resources");
	// *****************
	draw.load_fonts("fonts.conf");
	draw.apply_camera(b2Vec2(0, 0), 1, 1.5 * b2_pi);
	LoadingScreen loading(&draw, gui);
	loading.step(0, "Loading textures...");	
	//draw.fullscreen_toggle();
	draw.load_textures("textures.conf");
	loading.step(25, "Loading setup...");
	game.load_setup("setup.conf");
	audio.set_draw(&draw);
	loading.step(50, "Loading sounds...");
	audio.load_sounds();
	loading.step(75, "Loading tracks...");
	audio.load_musics();
	game.set_draw(&draw);
	game.set_gui(&gui);
	game.set_audio(&audio);
	loading.close();
	// SFML key names
	for (int i = 0; i < keyboard.names.size(); i++) {
		key_names.insert({keyboard.names[i], i});
	}
	keyboard.text_entered = &text_entered;
	menu_processing.init(gui, &draw, &mouse_pos, &keyboard, &network, &game, &replay, &reload);
	// Init hud
	hud = HUDProcessing(gui, &draw, &mouse_pos, &keyboard, &game, &network, &frame_marks);
	// Music name
	track = audio.get_music_by_number(aux::random_int(0, 131213));
	draw.display();
	draw.fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(draw.get_window()->getSize())),
		sf::Color(0, 0, 0, 255), 0);
	// Sleep(10000);
	// Dt
	game.set_dt(delay * 0.001);
	game.load_parameters("parameters.conf");
	load_token("token.conf");
}

int Control::get_is_running() {
	return is_running;
}

void Control::step() {
	//std::cout << replay.get_replay_frame()->get_change_vel() << " " << replay.get_replay_frame()->get() << "\n";
	// load configs
	if (reload) {
		menu_processing.save_keys("keys.conf", menu_processing.keys_menu_vec);
		load_keys("keys.conf");
	}
	// Receiving
	network.receive();

	// Delay check
	int time_current = aux::get_milli_count();
	int time_delta = time_current - time_prev;
	if (time_delta >= delay) {
		time_prev = time_current;
		// Set game dt for inner use (for replay) TODO: specific mode for real-time sync
		game.set_dt(time_delta * 0.001);

		// Pass message to game object
		if (replay.get_replay_active())
			game.update_state(replay.get_cur_frame());
		else
			game.update_state(network.get_message());
		// Draw		
		game.display(network.get_id());
		//FPS
		frame_marks.push(aux::get_milli_count());
		// HUD
		Camera camera_backup = *game.get_draw()->get_camera();
		game.get_draw()->apply_camera(b2Vec2(0, 0), 1, 1.5 * b2_pi);
		menu_processing.step();
		if (!menu_processing.active) {
			hud.step(gui);
		}
		gui.draw();

		// Restore camera
		game.get_draw()->set_camera(camera_backup);
		game.get_draw()->display();

		// Event processing
		process_events(draw.get_window());
		process_commands();

		// Sending
		network.send(commands_to_string());

		// Music
		if ((game.get_ship(network.get_id()) != nullptr) &&
			(game.get_ship(network.get_id())->get_player() != nullptr) &&
			(game.get_ship(network.get_id())->get_player()->get_is_alive())) {
			if (respawned)
			{
				audio.stop_music(track);
				track = audio.get_music_by_number(aux::random_int(0, 131213));
				audio.start_music(track);
			}
			audio.update_music(track, 1);
			respawned = 0;
		}
		else {
			respawned = 1;
			audio.update_music(track, 1.0 / 8);
		}
	}

	if (reload) {
		menu_processing.save_keys("keys.conf", menu_processing.keys_menu_vec);
		load_keys("keys.conf");
		load_config("client_config.conf");
		reload = 0;
	}

	if (draw.get_window() == nullptr || !draw.get_window()->isOpen())
		is_running = 0;
}

int Control::load_keys(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	std::string symbol;
	while (config >> symbol) {
		if (key_matches.find(symbol) != key_matches.end()) {
			// Command with such a name exists
			std::string symbol_1;
			std::vector<int> keys;
			while (config >> symbol_1) {
				if (symbol_1 == "END")
					break;
				if (key_names.find(symbol_1) != key_names.end()) {
					keys.push_back(key_names[symbol_1]);
					continue;
				}
				std::cerr << "Control::load_keys: unknown symbol " << symbol << "\n";
				return false;
			}
			key_matches[symbol] = keys;
			continue;
		}
		std::cerr << "Control::load_keys: unknown symbol " << symbol << "\n";
		return false;
	}
	return true;
}

void Control::load_config(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	std::string address;
	config >> address;
	network.set_server(address);
	int port_;
	config >> port_;
	network.set_port(port_);
	int id_;
	config >> id_;
	network.set_id(id_);
	std::string name_;
	config >> name_;
	network.set_name(name_);
	// Key config
	load_keys("keys.conf");
	game.load_setup("setup.conf");
}

void Control::load_token(std::string path) {
	std::ifstream file_to_comment(path);
	std::stringstream config = aux::comment(file_to_comment);
	int token;
	config >> token;
	if (token == 0) {
		token = aux::random_int(0, 10000000);
		std::ofstream fout;
		fout.open(path);
		fout << token;
		fout.close();
	}
	network.set_token(token);
}
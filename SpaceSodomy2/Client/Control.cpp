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
			command_module.set_command(Command_Module::ENGINE_LIN_FORWARD, 1);
		if (key_by_name("ENGINE_LIN_BACKWARD"))
			command_module.set_command(Command_Module::ENGINE_LIN_BACKWARD, 1);
		if (key_by_name("ENGINE_LIN_LEFT"))
			command_module.set_command(Command_Module::ENGINE_LIN_LEFT, 1);
		if (key_by_name("ENGINE_LIN_RIGHT"))
			command_module.set_command(Command_Module::ENGINE_LIN_RIGHT, 1);
		if (key_by_name("ENGINE_ANG_LEFT"))
			command_module.set_command(Command_Module::ENGINE_ANG_LEFT, 1);
		if (key_by_name("ENGINE_ANG_RIGHT"))
			command_module.set_command(Command_Module::ENGINE_ANG_RIGHT, 1);
		if (key_by_name("STABILIZE_ROTATION") || stabilize_rotation_auto)
			command_module.set_command(Command_Module::STABILIZE_ROTATION, 1);
		if (key_by_name("STABILIZE_ROTATION_AUTO") && !key_prev_by_name("STABILIZE_ROTATION_AUTO"))
			stabilize_rotation_auto = !stabilize_rotation_auto;
		if (key_by_name("SHOOT"))
			command_module.set_command(Command_Module::SHOOT, 1);
		if (key_by_name("BOOST"))
			command_module.set_command(Command_Module::BOOST, 1);
		if (key_by_name("RESPAWN"))
			command_module.set_command(Command_Module::RESPAWN, 1);
		if (key_by_name("BONUS_ACTIVATION"))
			command_module.set_command(Command_Module::BONUS_ACTIVATION, 1);
	}
	if (key_by_name("FULLSCREEN"))
		draw.fullscreen_toggle();

	// Zoom out
	if (commands_active) {
		if (key_by_name("ZOOM_OUT"))
			draw.get_camera()->modify_scale(pow(2, -delay * 0.001 * zoom_vel));
		if (key_by_name("ZOOM_IN"))
			draw.get_camera()->modify_scale(pow(2, delay * 0.001 * zoom_vel));
	}

	// Menu
	if (key_by_name("MENU") && !key_prev_by_name("MENU"))
		menu_processing.active = !menu_processing.active;
}

std::string Control::commands_to_string() {
	std::string message = "";
	message += game.get_gun_name() + " ";
	message += game.get_hull_name() + " ";
	message += "#";
	for (int i = 0; i < Command_Module::COMMAND_COUNT; i++) {
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
	draw.load_fonts("fonts.conf");
	draw.apply_camera(b2Vec2(0, 0), 1, 1.5 * b2_pi);
	draw.fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(draw.get_window()->getSize())),
		sf::Color(0, 0, 0, 255), 0);
	draw.text("Loading...", "font", { 0, -50 }, 50, sf::Color::White);
	Constant_Text loading_stage;
	loading_stage.set_color(sf::Color::Green);
	loading_stage.set_draw(&draw);
	loading_stage.set_pos({ 0, 50 });
	loading_stage.set_text_character_pixel_size(30);
	loading_stage.set_text("Loading textures...");
	loading_stage.step();
	Bar progress_bar;
	progress_bar.set_back_color(sf::Color(140, 140, 140, 255));
	progress_bar.set_front_color(sf::Color(200, 200, 200, 255));
	progress_bar.set_scale({ 700, 50 });
	progress_bar.set_pos({ 0, 0 });
	progress_bar.set_character_size(0);
	progress_bar.set_draw(&draw);
	progress_bar.set_max_value(100);
	progress_bar.set_value(0);
	progress_bar.step();
	draw.display();
	draw.fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(draw.get_window()->getSize())),
		sf::Color(0, 0, 0, 255), 0);
	//draw.fullscreen_toggle();
	draw.load_textures("textures.conf");
	loading_stage.set_text("Loading setup...");
	loading_stage.step();
	progress_bar.set_value(25);
	progress_bar.step();
	draw.text("Loading...", "font", { 0, -50 }, 50, sf::Color::White);
	draw.display();
	draw.fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(draw.get_window()->getSize())),
		sf::Color(0, 0, 0, 255), 0);
	game.load_setup("setup.conf");
	audio.set_draw(&draw);
	loading_stage.set_text("Loading sounds...");
	loading_stage.step();
	progress_bar.set_value(50);
	progress_bar.step();
	draw.text("Loading...", "font", { 0, -50 }, 50, sf::Color::White);
	draw.display();
	draw.fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(draw.get_window()->getSize())),
		sf::Color(0, 0, 0, 255), 0);
	audio.load_sounds();
	loading_stage.set_text("Loading tracks...");
	loading_stage.step();
	progress_bar.set_value(75);
	progress_bar.step();
	draw.text("Loading...", "font", { 0, -50 }, 50, sf::Color::White);
	draw.display();
	audio.load_musics();
	game.set_draw(&draw);
	game.set_audio(&audio);
	// Default key matches
	key_matches["ENGINE_LIN_FORWARD"] = { sf::Keyboard::W, sf::Keyboard::Up };
	key_matches["ENGINE_LIN_BACKWARD"] = { sf::Keyboard::S, sf::Keyboard::Down };
	key_matches["ENGINE_LIN_LEFT"] = { sf::Keyboard::Left };
	key_matches["ENGINE_LIN_RIGHT"] = { sf::Keyboard::Right };
	key_matches["ENGINE_ANG_LEFT"] = { sf::Keyboard::A };
	key_matches["ENGINE_ANG_RIGHT"] = { sf::Keyboard::D };
	key_matches["STABILIZE_ROTATION"] = { sf::Keyboard::D };
	key_matches["BOOST"] = { sf::Keyboard::LShift };
	key_matches["ZOOM_IN"] = { sf::Keyboard::E };
	key_matches["ZOOM_OUT"] = { sf::Keyboard::Q };
	key_matches["SHOOT"] = { sf::Keyboard::Space };
	key_matches["RESPAWN"] = { sf::Keyboard::R };
	key_matches["MENU"] = { sf::Keyboard::Escape };
	// SFML key names
	for (int i = 0; i < keyboard.names.size(); i++) {
		key_names.insert({keyboard.names[i], i});
	}
	keyboard.text_entered = &text_entered;
	menu_processing.init(&draw, &mouse_pos, &keyboard, &reload, &game);
	// Music name
	track = audio.get_music_by_number(aux::random_int(0, 131213));
	draw.display();
	draw.fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(draw.get_window()->getSize())),
		sf::Color(0, 0, 0, 255), 0);
	// Sleep(10000);
	// Dt
	game.set_dt(delay * 0.001);
	game.load_parameters("parameters.conf");
}

int Control::get_is_running() {
	return is_running;
}

void Control::step() {
	// load configs
	if (reload) {
		load_config("client_config.conf");
	}
	// Receiving
	network.receive();

	// Delay check
	int time_current = aux::get_milli_count();
	if (time_current - time_prev > delay) {
		time_prev = time_current;

		// Pass message to game object
		game.decode(network.get_message());

		// Draw		
		game.display(network.get_id());
		// Camera backup
		Camera camera_backup = *game.get_draw()->get_camera();
		game.get_draw()->apply_camera(b2Vec2(0, 0), 1, 1.5 * b2_pi);
		menu_processing.step();
		if (!menu_processing.active) {
			hud.step();
		}
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
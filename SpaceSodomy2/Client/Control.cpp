#include "Control.h"

int Control::key_by_name(std::string name) {
	int sum = 0;
	for (auto k : key_matches[name])
		sum += keyboard.state_current[k];
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
	// TODO: do it in one std::map
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
	if (key_by_name("SHOOT"))
		command_module.set_command(Command_Module::SHOOT, 1);

	// Zoom out
	if (key_by_name("ZOOM_OUT"))
		draw.get_camera()->modify_scale(pow(2, -delay * 0.001 * zoom_vel));
	if (key_by_name("ZOOM_IN"))
		draw.get_camera()->modify_scale(pow(2, delay * 0.001 * zoom_vel));
}

std::string Control::commands_to_string() {
	std::string message = "";
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
	draw.load_textures("textures.conf");
	draw.load_fonts("fonts.conf");
	game.set_draw(&draw);
	// Default key matches
	key_matches["ENGINE_LIN_FORWARD"] = { sf::Keyboard::W, sf::Keyboard::Up };
	key_matches["ENGINE_LIN_BACKWARD"] = { sf::Keyboard::S, sf::Keyboard::Down };
	key_matches["ENGINE_LIN_LEFT"] = { sf::Keyboard::Left};
	key_matches["ENGINE_LIN_RIGHT"] = { sf::Keyboard::Right};
	key_matches["ENGINE_ANG_LEFT"] = { sf::Keyboard::A};
	key_matches["ENGINE_ANG_RIGHT"] = { sf::Keyboard::D};
	key_matches["ZOOM_IN"] = { sf::Keyboard::E};
	key_matches["ZOOM_OUT"] = { sf::Keyboard::Q};
	key_matches["SHOOT"] = { sf::Keyboard::Space};
	// SFML key names
	std::vector<std::string> names = { "A", "B", "C", "D", "E", "F", "G", "H", 
		"I", "J", "K", "L", "M", "N", "O", "P", "Q", 
		"R", "S", "T", "U", "V", "W", "X", "Y", "Z", 
		"Num0", "Num1", "Num2", "Num3", "Num4", "Num5", 
		"Num6", "Num7", "Num8", "Num9", "Escape", "LControl", 
		"LShift", "LAlt", "LSystem", "RControl", "RShift", 
		"RAlt", "RSystem", "Menu", "LBracket", "RBracket", 
		"Semicolon", "Comma", "Period", "Quote", "Slash", 
		"Backslash", "Tilde", "Equal", "Hyphen", "Space", 
		"Enter", "Backspace", "Tab", "PageUp", "PageDown", 
		"End", "Home", "Insert", "Delete", "Add", "Subtract", 
		"Multiply", "Divide", "Left", "Right", "Up", "Down", 
		"Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4", 
		"Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9", 
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", 
		"F10", "F11", "F12", "F13", "F14", "F15", "Pause" };
	for (int i = 0; i < names.size(); i++) {
		key_names.insert({names[i], i});
	}
	keyboard.text_entered = &text_entered;
	menu_processing.init(&draw, &mouse_pos, &keyboard, &reload);
}

int Control::get_is_running() {
	return is_running;
}

void Control::step() {
	// load configs
	if (reload) {
		load_config("client_config.conf");
		reload = 0;
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
		menu_processing.step();
		game.get_draw()->display();

		// Event processing
		process_events(draw.get_window());
		process_commands();

		// Sending
		network.send(commands_to_string());
	}
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
}
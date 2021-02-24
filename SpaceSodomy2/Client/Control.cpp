#include "Control.h"

int Control::key_by_name(std::string name) {
	int sum = 0;
	for (auto k : key_matches[name])
		sum += keyboard.state_current[k];
	return sum;
}

void Control::process_events(sf::Window* window) {
	window->setKeyRepeatEnabled(false);
	sf::Event event;

	while (window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			is_running = 1;
			window->close();
			break;
		}
	}

	// Processing keyboard
	keyboard.state_prev = keyboard.state_current;
	for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
		keyboard.state_current[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
	}
}

void Control::process_commands() {
	command_module.reset();
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
}

int Control::get_is_running() {
	return is_running;
}

void Control::step() {
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

		// Event processing
		process_events(draw.get_window());
		process_commands();

		// Sending
		network.send(commands_to_string());
	}
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
	network.set_name(name_);
}
void Control::save_config(std::string path, std::string address_, int port_, int id_, std::string name_) {
	std::ofstream fout;
	fout.open(path);
	fout << address_ << " " << port_ << " " << id_ << " " << name_;
	fout.close();
}
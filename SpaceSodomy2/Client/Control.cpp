#include "Control.h"

void Control::process_events(sf::Window* window) {
	window->setKeyRepeatEnabled(false);
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

	// Processing keyboard
	keyboard.state_prev = keyboard.state_current;
	for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
		keyboard.state_current[i] = sf::Keyboard::isKeyPressed((sf::Keyboard::Key)i);
	}
}

void Control::process_commands() {
	command_module.reset();
	if (keyboard.state_current[sf::Keyboard::W] || keyboard.state_current[sf::Keyboard::Up])
		command_module.set_command(Command_Module::ENGINE_LIN_FORWARD, 1);
	if (keyboard.state_current[sf::Keyboard::S] || keyboard.state_current[sf::Keyboard::Down])
		command_module.set_command(Command_Module::ENGINE_LIN_BACKWARD, 1);
	if (keyboard.state_current[sf::Keyboard::Left])
		command_module.set_command(Command_Module::ENGINE_LIN_LEFT, 1);
	if (keyboard.state_current[sf::Keyboard::Right])
		command_module.set_command(Command_Module::ENGINE_LIN_RIGHT, 1);
	if (keyboard.state_current[sf::Keyboard::A])
		command_module.set_command(Command_Module::ENGINE_ANG_LEFT, 1);
	if (keyboard.state_current[sf::Keyboard::D])
		command_module.set_command(Command_Module::ENGINE_ANG_RIGHT, 1);
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
	keyboard.text_entered = &text_entered;
	menu_processing.init("menu.conf", &draw, &mouse_pos, &keyboard);
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
		menu_processing.step();
		game.get_draw()->display();

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
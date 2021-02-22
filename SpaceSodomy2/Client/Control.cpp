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

Control::Control() {
	draw.create_window(600, 600, "Space Sodomy II");
	draw.load_textures("textures.conf");
	game.set_draw(&draw);
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

		// Event processing
		process_events(draw.get_window());
		process_commands();

		// Draw
		for (int i = 0; i < Command_Module::COMMAND_COUNT; i++) {
			game.apply_command(0, i, command_module.get_command(i));
			std::cout << command_module.get_command(i);
		}
		std::cout << "\n";
		
		game.step(delay * 0.001);
		game.display();


		// Sending
	}
}

#include "Control.h"

Control::Control() {
	draw.create_window(600, 600, "Space Sodomy II");
	game.set_draw(&draw);
}

void Control::step() {
	// Receiving
	network.receive();

	// Delay check
	int time_current = aux::get_milli_count();
	if (time_current - time_prev > delay) {
		std::cout << "q\n";
		time_prev = time_current;

		// Pass message to game object
		game.decode(network.get_message());

		// Draw
		game.display();

		// TODO: event processing

		// Sending
	}
}

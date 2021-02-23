#include "Control.h"	

Control::Control() {
	//loading data from config
	game.load_walls("level.lvl");

	//***************************
	//TODO: load data from config
	//***************************
}

void Control::receive() {
	network.receive();
	if (network.get_last_message() == "")
		return;
	// Splitting message
	std::stringstream message;
	message << network.get_last_message();
	network.del_last_message();
	std::string IP_address_, local_, name_;
	message >> IP_address_;
	int id_;
	message >> id_;
	message >> local_;
	message >> name_;
	//std::cout << IP_address_ << " " << local_ << "\n";
	// Adding a new player to the base & to the game 
	if (!addresses.count(IP_by_id[id_])) {
		addresses.insert(IP_address_);
		IP_by_id[id_] = IP_address_;
		id_by_IP[IP_address_] = id_;
		time_by_IP[IP_address_] = aux::get_milli_count();
		sf::Color new_color;
		new_color.r = rand() % 256;
		new_color.g = rand() % 256;
		new_color.b = rand() % 256;
		game.create_player(id_, new_color, name_, b2Vec2_zero, 0);
	}
	// Applying commands
	if (IP_by_id[id_] == IP_address_) {
		std::string command_string;
		message >> command_string;
		for (int i = 1; i < command_string.size(); i++) {
			game.apply_command(id_, i - 1, command_string[i] == '1');
		}
	}
}

void Control::step() {
	// Receiving data
	receive();
	// Check if the time for the next update has come
	if (aux::get_milli_count() - last_step_time >= delay) {
		last_step_time += delay;

		// Banning disconnected players
		std::set <std::string> banned;
		for (auto it = addresses.begin(); it != addresses.end(); it++) {
			if (aux::get_milli_count() - time_by_IP[*it] >= ban)
				banned.insert(*it);
		}
		for (auto address : banned) {
			game.del_player(id_by_IP[address]);
			addresses.erase(address);
			network.del_address(address);
		}
		// Release next game step 
		game.step(delay * 0.001);
		// Send encoded info;
		network.send(game.encode());
	}
}
#include "Control.h"

void Control::load_config(std::string path) {
	std::ifstream fileInput(path);
	std::stringstream file = aux::comment(fileInput);

	std::string command; // Current command
	while (file >> command) {
		if (command == "END") // End of file
			break;

		if (command == "PORT") {
			int port_;
			file >> port_;
			network.set_port(port_);
		}

		if (command == "MAP") {
			std::string name;
			file >> name;
			game.load_map(name);	
		}

		if (command == "PARAMETERS") {
			std::string name;
			file >> name;
			game.load_parameters(name);
		}
	}
}

Control::Control() {
	//loading data from config
	load_config("config.conf");
}

void Control::receive() {
	
	network.receive();
	if (network.get_last_message() == "")
		return;
	// Splitting message
	std::stringstream message;
	message << network.get_last_message();
	//std::cout << network.get_last_message() << "\n";
	network.del_last_message();
	std::string IP_address_, name_, time, gun_name, hull_name;
	message >> IP_address_;
	int id_;
	message >> id_;
	message >> time;
	message >> name_;
	message >> gun_name;
	message >> hull_name;
	//std::cout << IP_address_ << " " << local_ << "\n";
	// Adding a new player to the base & to the game 
	if (!addresses.count(IP_by_id[id_])) {
		addresses.insert(IP_address_);
		IP_by_id[id_] = IP_address_;
		id_by_IP[IP_address_] = id_;
		time_by_id[id_] = aux::get_milli_count();
		sf::Color new_color = aux::from_hsv(aux::random_int(0, 360), 1, 1);
		game.new_player(id_, new_color, name_, gun_name, hull_name);
	}
	// Applying commands
	if (IP_by_id[id_] == IP_address_) {
		time_by_id[id_] = aux::get_milli_count();
		std::string command_string;
		message >> command_string;
		game.player_by_id(id_)->set_name(name_);
		if (!game.player_by_id(id_)->get_is_alive()) {
			game.player_by_id(id_)->set_gun_name(gun_name);
			game.player_by_id(id_)->set_hull_name(hull_name);
		}
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
		std::set <int> banned;
		for (auto id : IP_by_id) {
			if (aux::get_milli_count() - time_by_id[id.first] >= disconnect_timeout)
				banned.insert(id.first);
		}
		for (auto id : banned) {
			//game.delete_player(id);
			//IP_by_id.erase(IP_by_id.find(id));
			//addresses.erase(IP_by_id[id]);
			//network.del_address(IP_by_id[id]);
		}
		// Release next game step 
		game.step(delay * 0.001);
		// Send encoded info;
		network.send(game.encode());
	}
}
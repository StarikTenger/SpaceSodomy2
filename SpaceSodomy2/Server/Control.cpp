#include "Control.h"

Control::Control() {
	//loading data from config
	//***************************
	//TODO: load data from config
	//***************************
}

void Control::receive() {
	network.receive();
	// Splitting message
	std::stringstream message;
	message << network.get_last_message();
	network.del_last_message();
	std::string IP_adress_, local_, name_;
	message >> IP_adress_;
	message >> local_;
	int id_ = atoi(local_.c_str());
	message >> local_;
	message >> name_;
	message >> local_;
	// Adding a new player to the base & to the game 
	if (!addresses.count(IP_by_id[id_])) {
		addresses.insert(IP_adress_);
		IP_by_id[id_] = IP_adress_;
		time_by_IP[IP_adress_] = aux::get_milli_count();
		sf::Color new_color;
		new_color.r = rand() % 256;
		new_color.g = rand() % 256;
		new_color.b = rand() % 256;
		game.create_player(id_, new_color, name_, b2Vec2_zero, 0);
	}
	// Applying commands
	if (IP_by_id[id_] == IP_adress_)
		for (int i = 0; message >> local_; i++) {
			game.apply_command(id_, i, atoi(local_.c_str()));
		}
}

void Control::step() {
	// Receiving data
	receive();
	// Check if the time for the next update has come
	if (aux::get_milli_count() - last_step_time >= delay) {
		// Banning disconnected players
		std::set <std::string> banned;
		for (auto it = addresses.begin(); it != addresses.end(); it++) {
			if (aux::get_milli_count() - time_by_IP[*it] >= ban)
				banned.insert(*it);
		}
		for (auto address : banned) {
			addresses.erase(address);
			network.del_address(address);
		}
		// Release next game step 
		game.step(delay * 0.01);
		// Send encoded info
		network.send(game.encode());
	}
}
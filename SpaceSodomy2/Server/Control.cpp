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
			//sys.currentMap = name;
			game.load_walls("maps/" + name + ".lvl");	
		}

		/* TODO:
		if (command == "TEAM") {
			int id;
			file >> id;
			sys.teams.insert({ id, {} });
			std::string command1;
			while (file >> command1) {
				if (command1 == "END")
					break;


				if (command1 == "S") {
					Vec2 pos;
					file >> pos.x >> pos.y;
					sys.teams[id].spawnpoints.push_back(pos);
				}
				if (command1 == "COL") {
					Color col;
					file >> col.r >> col.g >> col.b;
					col.a = 255;
					sys.teams[id].color = col;
				}
			}
		}
		if (command == "PLAYER") {
			int id, team;
			file >> id >> team;
			sys.setPlayer({ id, team, {},{0, 0}, 1, 0 });
		}
		if (command == "PARAMETERS") {
			std::string command1;
			while (file >> command1) {
				if (command1 == "END")
					break;

				if (command1 == "PLAYER_HP")
					file >> sys.parameters.player_hp;
				if (command1 == "PLAYER_ENERGY")
					file >> sys.parameters.player_energy;
				if (command1 == "PLAYER_STAMINA")
					file >> sys.parameters.player_stamina;
				if (command1 == "PLAYER_RECOVERY")
					file >> sys.parameters.player_stamina_recovery;

			}
		}
		if (command == "BONUSINFO") {
			std::string command1;
			while (file >> command1) {
				if (command1 == "END")
					break;


				int type = System::bonusNames[command1];
				file >> sys.bonusInfo[type].limit;
				file >> sys.bonusInfo[type].countdownTime;
			}
		}
		if (command == "MODULEINFO") {
			std::string command1;
			while (file >> command1) {
				if (command1 == "END")
					break;


				int type = System::moduleNames[command1];
				;
				file >> sys.moduleInfo[type].cooldownTime; // Period
				file >> sys.moduleInfo[type].energy; // Energy consumption
				file >> sys.moduleInfo[type].stamina; // Stamina consumption
			}
		}*/
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
		new_color.a = 255;
		game.create_player(id_, new_color, name_, b2Vec2_zero, 0);
	}
	// Applying commands
	if (IP_by_id[id_] == IP_address_) {
		time_by_IP[IP_address_] = aux::get_milli_count();
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
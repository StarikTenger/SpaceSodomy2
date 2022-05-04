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
			std::cout << "MAP loaded\n";

		}

		if (command == "PARAMETERS") {
			std::string name;
			file >> name;
			game.load_parameters(name);
			std::cout << "PARAMETERS loaded\n";
		}

		if (command == "BOT_LIST") {
			std::string name;
			file >> name;
			load_bots(name);
			std::cout << "BOT_LIST loaded\n";
		}

	}
}

Control::Control() {
	//loading data from config
	load_config("config.conf");
	time_t now = time(0);
	auto dt = localtime(&now);
	network.set_replay_path("replays/" + std::to_string(dt->tm_mday) + "." + ((dt->tm_mon + 1 <10)?
		"0" + std::to_string(dt->tm_mon + 1) : std::to_string(dt->tm_mon + 1)) + "." + std::to_string(dt->tm_year + 1900) +
		"_" + std::to_string(dt->tm_hour) + "." + std::to_string(dt->tm_min) + ".rep");
	game.set_time(&time_by_id);
}

bool Control::load_bots(std::string path) {
	std::ifstream file_input(path);
	std::stringstream input = aux::comment(file_input);

	std::string symbol;
	while (input >> symbol) {
		if (symbol == "END")
			break;

		auto read_symbol = [&](std::string symbol_name, auto& var) -> bool {
			if (symbol == symbol_name) {
				decltype(var) val(var);
				if (!(input >> val)) {
					std::cerr << "Control::load_bots: failed to read " + symbol_name + "\n";
					return false;
				}
				var = val;
			}
			return true;
		};

		if (symbol == "BOT") {
			ShipBrain::Equip equip;
			std::string name;

			while (input >> symbol) {
				if (symbol == "END")
					break;
				
				read_symbol("NAME", name);
				read_symbol("GUN_NAME", equip.gun_name);
				read_symbol("HULL_NAME", equip.hull_name);
				read_symbol("LEFT_MODULE_NAME", equip.left_module_name);
				read_symbol("RIGHT_MODULE_NAME", equip.right_module_name);
			}

			BotControl* bot = new BotControl(name, ShipBrain::Type::EDGAR_BRAIN, game.get_readable());
			bot->set_equip(name, equip);
			bots.push_back(bot);
		};
	}
	return true;
}

void  Control::parse_message(std::stringstream &message) {
	// Received params
	std::string IP_address_, name_, gun_name, hull_name, left_module, right_module;
	int id_, token;

	message >> IP_address_;
	message >> id_;
	{
		std::string time;
		message >> time; // discard
	}
	message >> name_;
	message >> token;
	message >> gun_name;
	message >> hull_name;
	message >> left_module >> right_module;


	// Adding a new player to the base & to the game 
	if (!addresses.count(IP_address_) && (!token_by_id[id_] || (token == token_by_id[id_]))) {
		addresses.insert(IP_address_);
		IP_by_id[id_] = IP_address_;
		id_by_IP[IP_address_] = id_;
		token_by_id[id_] = token;
		time_by_id[id_] = aux::get_milli_count();


		PlayerDef def(id_, name_);
		def.color = aux::from_hsv(aux::random_int(0, 360), 1, 1);
		def.gun_name = gun_name;
		def.hull_name = hull_name;
		def.left_module_name = left_module;
		def.right_module_name = right_module;
		game.new_player(def);
	}

	// Applying commands
	if (token_by_id[id_] == token) {
		IP_by_id[id_] = IP_address_;
		id_by_IP[IP_address_] = id_;
		time_by_id[id_] = aux::get_milli_count();
		std::string command_string;
		message >> command_string;
		game.player_by_id(id_)->set_name(name_);
		if (!game.player_by_id(id_)->get_is_alive()) {
			game.player_by_id(id_)->set_gun_name(gun_name);
			game.player_by_id(id_)->set_hull_name(hull_name);
			game.player_by_id(id_)->set_left_module_name(left_module);
			game.player_by_id(id_)->set_right_module_name(right_module);
		}
		for (int i = 1; i < command_string.size(); i++) {
			game.apply_command(id_, i - 1, command_string[i] == '1');
		}
	}
}

void Control::receive() {
	network.receive();
	if (network.get_last_message() == "")
		return;
	// Splitting message
	std::stringstream message;
	message << network.get_last_message();
	network.del_last_message();

	parse_message(message);
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
		for (int i = 0; i < bots.size(); i++) {
			std::stringstream message;
			message << bots[i]->get_message();
			parse_message(message);
		}
		game.step(delay * 0.001);
		// Send encoded info;
		network.send(game.encode());
	}
}
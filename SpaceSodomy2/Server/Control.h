#pragma once
#include <AuxLib/AuxLib.h>
#include <Game\Game.h>
#include <Network\Server_Network.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <random>

class Control {
private:
	// Init game & network
	Game game;
	Server_Network network;
	int delay = 20; // delay between game updates
	int ban = 1000; // time for reconnection
	int last_step_time = aux::get_milli_count(); // last step time
	std::set <std::string> addresses; // IP addresses of active players
	std::map <int, std::string> IP_by_id; // getting IP by id
	std::map <std::string, int> id_by_IP; // getting id by IP
	std::map <std::string, int> time_by_IP; // getting last connection time by IP
public:
	Control();

	void receive();
	void step();
};
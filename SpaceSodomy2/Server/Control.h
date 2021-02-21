#pragma once
#include <AuxLib/AuxLib.h>
#include <Game\Game.h>
#include <Network\Server_Network.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>

class Control {
private:
	Game game;
	Server_Network network;
	int delay = 20;
	int last_step_time = aux::get_milli_count();
	std::set <std::string> addresses;
	std::map <int, std::string> IP_by_id;
	std::map <std::string, int> time_by_IP;
public:
	Control();

	void receive();
	void step();
};
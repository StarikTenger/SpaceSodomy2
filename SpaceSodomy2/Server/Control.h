#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <AuxLib/AuxLib.h>
#include <Game\Game.h>
#include <Network\ServerNetwork.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <random>
#include <ctime>
#include <Control/Control.h>

class Control : public iControl {
private:
	// Init game & network
	Game game;
	ServerNetwork network;
	int delay = 20; // delay between game updates
	int disconnect_timeout = 1000; // time for reconnection
	int time_prev = aux::get_milli_count(); // last step time
	std::set <std::string> addresses; // IP addresses of active players
	std::map <int, std::string> IP_by_id; // getting IP by id
	std::map <std::string, int> id_by_IP; // getting id by IP
	std::map <int, int> id_by_token; // getting id by token
	std::map <int, int> token_by_id; // getting token by id
	std::map <int, int> time_by_id; // getting last connection time by IP

public:
	Control();

	void load_config(std::string path) override; // loading configurations
	void receive();
	void outer_step() override;
	void inner_step(int time_time_delta) override;
};
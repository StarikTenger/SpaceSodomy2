#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Network/Client_Network.h>
#include <AuxLib/AuxLib.h>
#include <Draw/Draw.h>
#include "Game_Client.h"
#include <string>
#include <fstream>

class Control {
private:
	// 0 means the program is supposed to be stopped
	int is_running = 1;
	// Period of drawing & sending in ms
	int delay = 20;
	// Last time of activity
	int time_prev = 0;
	// Zoom velocity
	float zoom_vel = 2;
	// Current player's commands
	Command_Module command_module;
	// Connection to server, contains id, name, server adress & port
	Client_Network network;
	// Contains game objects
	Game_Client game;
	// Contains window 7 other drawing stuff
	Draw draw;

	// Keyboard structure
	struct Keyboard {
		std::vector<int> state_current = std::vector<int>(sf::Keyboard::KeyCount);
		std::vector<int> state_prev = state_current;
	} keyboard;

	// Matches keys to name (command)
	std::map<std::string, std::vector<int>> key_matches;

	// Gets key by name
	int key_by_name(std::string name);

	// Events -> keyboard
	void process_events(sf::Window*);
	// Turns commands into the command module state
	void process_commands();
	// Converts commands into single string
	std::string commands_to_string();
public:
	// Default constructor
	Control();
	// Get methods
	int get_is_running();
	// Func to call regulary
	void step();
	// Config
	void load_config(std::string path);
	void save_config(std::string path, std::string address_, int port_, int id_, std::string name_);
};


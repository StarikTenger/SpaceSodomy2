#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Network/Client_Network.h>
#include <AuxLib/AuxLib.h>
#include <Draw/Draw.h>
#include "Menu_Processing.h"
#include "Game_Client.h"
#include <string>
#include <fstream>
#include <queue>

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
	// Contains menu objects
	Menu_Processing menu_processing;
	// Contains mouse position
	b2Vec2 mouse_pos;
	// Keyboard structure
	aux::Keyboard keyboard;
	// Text entered
	std::queue<wchar_t> text_entered;

	// SFML keys names
	std::map<std::string, int> key_names;
	// Matches keys to name (command)
	std::map<std::string, std::vector<int>> key_matches;

	// Gets key by name
	int key_by_name(std::string name);
	int key_prev_by_name(std::string name);
	

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
	// Loading keys
	int load_keys(std::string path);
	// Config
	void load_config(std::string path);
	bool reload = 1; // need to reload configs
};


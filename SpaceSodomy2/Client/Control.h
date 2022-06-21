#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Network/ClientNetwork.h>
#include <AuxLib/AuxLib.h>
#include <Draw/Draw.h>
#include <Audio/Audio.h>
#include "HUDProcessing.h"
#include "MenuProcessing.h"
#include "GameClient.h"
#include "Replay.h"
#include "LoadingScreen.h"
#include <string>
#include <fstream>
#include <queue>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>

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
	CommandModule command_module;
	// Connection to server, contains id, name, server adress & port
	ClientNetwork network;
	// Contains game objects
	GameClient game;
	// Contains window 7 other drawing stuff
	Draw draw;
	// Contains audio
	Audio audio;
	// TGUI object
	tgui::Gui gui;
	// Contains menu objects
	MenuProcessing menu_processing;
	// Contains mouse position
	b2Vec2 mouse_pos;
	// Keyboard structure
	aux::Keyboard keyboard;

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
	// HUD
	HUDProcessing hud;

	// Auto stabilize rotation
	bool stabilize_rotation_auto = 0;

	// Respawned
	bool respawned = 1;

	// Music name
	std::string track;

	// Replay
	Replay replay;

	// FPS
	std::queue<int> frame_marks;

	// Local server
	aux::Process server = aux::Process("Server.exe");

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
	void load_token(std::string path);
	bool reload = 1; // need to reload configs
};


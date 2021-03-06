#pragma once
#include <SFML/Graphics.hpp> 
#include <string>
#include "Command_Module.h"

class Player {
private:
	int id;
	sf::Color color;
	std::string name;
	Command_Module* command_module;
public:
	Player();
	Player(int id_, sf::Color color_, std::string name_);

	int get_id();
	sf::Color get_color();
	std::string get_name();
	Command_Module* get_command_module();
	
	void set_id(int id_);
	void set_color(sf::Color color_);
	void set_name(std::string name_);
	void set_command_module(Command_Module*);
};
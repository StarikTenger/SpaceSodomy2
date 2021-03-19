#pragma once
#include <SFML/Graphics.hpp> 
#include <string>
#include "Command_Module.h"
#include "Counter.h"
#include "iId.h"

class Player : public iId{
private:
	bool is_alive = 1;
	sf::Color color;
	std::string name;
	Command_Module* command_module = nullptr;
	Counter* time_to_respawn = nullptr;
public:
	Player();
	Player(int id_, sf::Color color_, std::string name_);

	bool get_is_alive();
	sf::Color get_color();
	std::string get_name();
	Command_Module* get_command_module();
	Counter* get_time_to_respawn();
	
	void set_is_alive(bool);
	void set_color(sf::Color color_);
	void set_name(std::string name_);
	void set_command_module(Command_Module*);
	void set_time_to_respawn(Counter*);
};
#pragma once
#include <SFML/Graphics.hpp> 
#include <string>
#include "Command_Module.h"
#include "Counter.h"
#include "iId.h"

class Player : public iId {
private:
	int deaths = 0;
	int kills = 0;
	bool is_alive = 1;
	sf::Color color;
	std::string name = "_";
	std::string gun_name = "default";
	std::string hull_name = "default";
	Command_Module* command_module = nullptr;
	Counter* time_to_respawn = nullptr;
public:
	Player();
	Player(int id_, sf::Color color_, std::string name_);

	bool get_is_alive();
	sf::Color get_color();
	std::string get_name();
	std::string get_gun_name();
	std::string get_hull_name();
	Command_Module* get_command_module();
	Counter* get_time_to_respawn();
	int get_deaths();
	int get_kills();
	
	void set_is_alive(bool);
	void set_color(sf::Color color_);
	void set_name(std::string name_);
	void set_gun_name(std::string val);
	void set_hull_name(std::string val);
	void set_command_module(Command_Module*);
	void set_time_to_respawn(Counter*);
	void set_deaths(int deaths_);
	void set_kills(int kills_);

	void add_death();
	void add_kill();
};
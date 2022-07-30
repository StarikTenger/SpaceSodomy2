#pragma once
#include <SFML/Graphics.hpp> 
#include <string>
#include "CommandModule.h"
#include "Counter.h"
#include "iId.h"
#include "iTeamMember.h"
#include <AuxLib/AuxLib.h>


class Player : public iTeamMember {
private:
	int deaths = 0;
	int kills = 0;
	bool is_alive = 1;
	sf::Color color;
	std::string name = "_";
	std::string gun_name = "default";
	std::string hull_name = "default";
	std::string left_module_name = "NONE";
	std::string right_module_name = "NONE";
	CommandModule* command_module = nullptr;
	Counter* time_to_respawn = nullptr;
	int ping = 0;
public:
	Player();
	Player(int id_, sf::Color color_, std::string name_);

	bool get_is_alive();
	sf::Color get_color();
	std::string get_name();
	std::string get_gun_name();
	std::string get_hull_name();
	std::string get_left_module_name();
	std::string get_right_module_name();
	CommandModule* get_command_module();
	Counter* get_time_to_respawn();
	int get_deaths();
	int get_kills();
	int get_ping();
	
	void set_is_alive(bool);
	void set_color(sf::Color color_);
	void set_name(std::string name_);
	void set_gun_name(std::string val);
	void set_hull_name(std::string val);
	void set_left_module_name(std::string val);
	void set_right_module_name(std::string val);
	void set_command_module(CommandModule*);
	void set_time_to_respawn(Counter*);
	void set_deaths(int deaths_);
	void set_kills(int kills_);
	void set_ping(int ping_);

	void add_death();
	void add_kill();
};

struct PlayerDef {
	int id;
	//Player::Type type;
	std::string name;
	sf::Color color = aux::from_hsv(aux::random_int(0, 360), 1, 1);
	std::string gun_name = "default";
	std::string hull_name = "default";
	std::string left_module_name = "NONE";
	std::string right_module_name = "NONE";

	PlayerDef(int _id, std::string _name) : id(_id), name(_name) {};
};
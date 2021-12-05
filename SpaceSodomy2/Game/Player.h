#pragma once
#include <SFML/Graphics.hpp> 
#include <string>
#include "Command_Module.h"
#include "Counter.h"
#include "iId.h"

class Team_Id {
	iId id;
public:
	Team_Id() = default;
	int get_team_id();
	void set_team_id(int _id);
};

class Combatant : public Team_Id, public iId {
	int deaths = 0;
	int kills = 0;
	std::string name = "_";
	sf::Color color = sf::Color::White;
	static bool is_friendly_fire; // TODO : non-global storage
public:
	static void set_friendly_fire(bool val);
	Combatant() = default;
	Combatant(int id_, sf::Color color_, std::string name_);
	int get_deaths();
	int get_kills();
	std::string get_name();
	sf::Color get_color();

	void set_deaths(int deaths_);
	void set_kills(int kills_);
	void set_color(sf::Color color_);
	void set_name(std::string);

	void add_death();
	void add_kill();
	void rm_kill();


	bool is_hostile_to(Combatant* other);
	bool is_deals_damage_to(Combatant* other);
};


class Player : public Combatant {
private:
	bool is_alive = 1;
	std::string gun_name = "default";
	std::string hull_name = "default";
	std::string left_module_name = "NONE";
	std::string right_module_name = "NONE";
	Command_Module* command_module = nullptr;
	Counter* time_to_respawn = nullptr;
	int ping = 0;
public:
	Player() = default;
	Player(int id_, sf::Color color_, std::string name_);

	bool get_is_alive();
	std::string get_gun_name();
	std::string get_hull_name();
	std::string get_left_module_name();
	std::string get_right_module_name();
	Command_Module* get_command_module();
	Counter* get_time_to_respawn();
	int get_ping();
	
	void set_is_alive(bool);
	void set_gun_name(std::string val);
	void set_hull_name(std::string val);
	void set_left_module_name(std::string val);
	void set_right_module_name(std::string val);
	void set_command_module(Command_Module*);
	void set_time_to_respawn(Counter*);
	void set_ping(int ping_);
};
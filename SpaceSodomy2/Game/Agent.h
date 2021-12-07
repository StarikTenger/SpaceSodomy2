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
	Combatant(int id_, int team_id_, sf::Color color_, std::string name_);
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
class Agent : public Combatant {
	Command_Module* command_module = nullptr;
public:
	Agent() = default;
	Agent(int id_, int team_id_, sf::Color color_, std::string name_);
	Command_Module* get_command_module();
	void set_command_module(Command_Module*);
};
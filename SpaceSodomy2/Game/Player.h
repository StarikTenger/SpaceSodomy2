#pragma once
#include <SFML/Graphics.hpp> 
#include <string>
#include "CommandModule.h"
#include "Counter.h"
#include "iId.h"
#include "ShipBrain.h"

class Player : public iId {
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
	ShipBrain* brain = nullptr; // Player must clean it up
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
	ShipBrain* get_brain();
	
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
	void set_brain(ShipBrain*);

	void add_death();
	void add_kill();
};

struct Player_Def {
	enum Type {
		NETWORK_PLAYER,
		EDGAR_BOT,
		COUNT
	};
	int id;
	Type type;
	std::string name;
	sf::Color color = sf::Color::Cyan;
	std::string gun_name = "default";
	std::string hull_name = "default";
	std::string left_module_name = "NONE";
	std::string right_module_name = "NONE";

	Player_Def(int _id, Type _type, std::string _name) : id(_id), type(_type), name(_name) {};
};
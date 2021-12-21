#pragma once
#include "Agent.h"

class Player : public Agent {
private:
	std::string gun_name = "default";
	std::string hull_name = "default";
	std::string left_module_name = "NONE";
	std::string right_module_name = "NONE";
	int ping = 0;
public:
	Player() = default;
	Player(int id_, int team_id_, sf::Color color_, std::string name_);

	std::string get_gun_name();
	std::string get_hull_name();
	std::string get_left_module_name();
	std::string get_right_module_name();
	int get_ping();
	
	void set_gun_name(std::string val);
	void set_hull_name(std::string val);
	void set_left_module_name(std::string val);
	void set_right_module_name(std::string val);
	void set_ping(int ping_);
};
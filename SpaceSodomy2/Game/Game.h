#pragma once
#include <vector>
#include <map>
#include <string>
#include "Ship.h"
#include "Engine.h"
#include <box2d/box2d.h>

class Game {
protected:
	// Objects' systems
	std::vector<Ship*> ships;
	std::vector<Engine*> engines;
	std::map<int, Command_Module*> command_modules;
	b2World physics = b2World(b2Vec2_zero);

	b2Body* create_round_body(b2Vec2 pos, float angle, float radius, float mass);
	Ship* create_ship(b2Vec2 pos, float angle, int id);

	 // Processing functions
	void process_engines();
public:
	Game();
	// Sets command to player with id=id
	void apply_command(int id, int command, int val);
	void step(float dt);
	// Encodes class into string
	std::string encode();
	// Decodes class from string
	void decode(std::string source);
};


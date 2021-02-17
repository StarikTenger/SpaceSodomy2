#pragma once
#include <vector>
#include <map>
#include "Ship.h"
#include "Engine.h"
#include <box2d/box2d.h>

class Game {
private:
	// Objects' systems
	std::vector<Ship*> ships;
	std::vector<Engine*> engines;
	std::map<int, Command_Module*> command_modules;
	b2World physics = b2World(b2Vec2_zero);

	b2Body* create_round_body(b2Vec2 pos, float angle, float radius, float mass);
	Ship* create_ship(b2Vec2 pos, float angle, int team);

	 // Processing functions
	void process_engines();
public:
	Game();
	void step1(float dt);
};


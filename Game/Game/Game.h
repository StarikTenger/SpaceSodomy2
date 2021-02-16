#pragma once
#include <vector>
#include "Ship.h"
#include "Engine.h"
#include <box2d/box2d.h>

class Game {
private:
	// Objects' systems
	std::vector<Ship> ships;
	std::vector<Engine> engines;
	b2World physics = b2World(b2Vec2_zero);


public:
	Game();
	void step(float dt);
};


#pragma once
#include <deque>
#include "Rocket.h"

// TODO: replace Rocket_Def* with Rocket_Def
class RocketManager {
private:
	std::deque<Rocket_Def> rockets_to_create;

public:
	// Constructor
	RocketManager();

	// Create projectile
	void create_rocket(Rocket_Def def);

	// Get another projectile
	int get_next(Rocket_Def&);
};


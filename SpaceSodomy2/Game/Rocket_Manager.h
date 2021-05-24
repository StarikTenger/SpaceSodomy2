#pragma once
#include <deque>
//#include "Rocket_Brain.h"

struct Rocket_Def;
// TODO: replace Rocket_Def* with Rocket_Def
class Rocket_Manager {
private:
	std::deque<Rocket_Def*> rockets_to_create;

public:
	// Constructor
	Rocket_Manager();

	// Create projectile
	void create_rocket(Rocket_Def* def);

	// Get another projectile
	int get_next(Rocket_Def*&);
};


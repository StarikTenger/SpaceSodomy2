#pragma once
#include <vector>
#include <deque>
#include "Projectile_Def.h"

class Projectile_Manager {
private:
	std::deque<Projectile_Def> projectiles_to_create;

public:
	// Constructor
	Projectile_Manager();

	// Create projectile
	void create_projectile(Projectile_Def projectile_def);

};


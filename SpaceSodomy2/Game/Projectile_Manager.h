#pragma once
#include <vector>
#include "Projectile.h"

class Projectile_Manager {
private:
	std::vector<Projectile*> projectiles;
	friend std::vector<Projectile*>& get_projectiles(Projectile_Manager& pm);
public:
	// Constructor
	Projectile_Manager();

	// TODO: Func thts create projectile
};


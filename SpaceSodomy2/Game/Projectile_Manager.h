#pragma once
#include <vector>
#include "Projectile.h"

class Projectile_Manager {
private:
	std::vector<Projectile*>* projectiles;

public:
	// Constructor
	Projectile_Manager();
	Projectile_Manager(std::vector<Projectile*>*);

	// Set methods
	void set_projectiles(std::vector<Projectile*>*);

	// Create projectile
	void create_projectile();

};


#pragma once
#include <vector>
#include <deque>
#include "ProjectileDef.h"

class ProjectileManager {
private:
	std::deque<ProjectileDef> projectiles_to_create;

public:
	// Constructor
	ProjectileManager();

	// Create projectile
	void create_projectile(ProjectileDef projectile_def);

	// Get another projectile
	int get_next(ProjectileDef&);
};


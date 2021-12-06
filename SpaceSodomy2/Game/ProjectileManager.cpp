#include "pch.h"
#include "ProjectileManager.h"

ProjectileManager::ProjectileManager() {
}

void ProjectileManager::create_projectile(ProjectileDef projectile_def) {
	projectiles_to_create.push_back(projectile_def);
}

int ProjectileManager::get_next(ProjectileDef& projectile_def) {
	if (!projectiles_to_create.size())
		return false;
	projectile_def = projectiles_to_create.front();
	projectiles_to_create.pop_front();
	return true;
}

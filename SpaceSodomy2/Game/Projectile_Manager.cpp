#include "pch.h"
#include "Projectile_Manager.h"

Projectile_Manager::Projectile_Manager() {
}

void Projectile_Manager::create_projectile(Projectile_Def projectile_def) {
	projectiles_to_create.push_back(projectile_def);
}

int Projectile_Manager::get_next(Projectile_Def& projectile_def) {
	if (!projectiles_to_create.size())
		return false;
	projectile_def = projectiles_to_create.front();
	projectiles_to_create.pop_front();
	return true;
}

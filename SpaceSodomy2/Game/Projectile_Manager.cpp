#include "pch.h"
#include "Projectile_Manager.h"

Projectile_Manager::Projectile_Manager() {
}

void Projectile_Manager::create_projectile(Projectile_Def projectile_def) {
	projectiles_to_create.push_back(projectile_def);
}

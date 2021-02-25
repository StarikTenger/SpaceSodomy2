#include "pch.h"
#include "Projectile_Manager.h"

Projectile_Manager::Projectile_Manager() {
}

std::vector<Projectile*>& get_projectiles(Projectile_Manager& pm) {
	return pm.projectiles;
}

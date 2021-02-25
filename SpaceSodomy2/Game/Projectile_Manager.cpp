#include "pch.h"
#include "Projectile_Manager.h"

Projectile_Manager::Projectile_Manager() {
}

Projectile_Manager::Projectile_Manager(std::vector<Projectile*>* _projectiles) {
	projectiles = _projectiles;
}

void Projectile_Manager::set_projectiles(std::vector<Projectile*>* val) {
	projectiles = val;
}

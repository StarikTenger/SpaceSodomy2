#include "pch.h"
#include "Projectile.h"

Projectile::Projectile() {
}

b2Body* Projectile::get_body() {
	return body;
}

Player* Projectile::get_player() {
	return player;
}

float Projectile::get_damage() {
	return damage;
}

void Projectile::set_body(b2Body* val) {
	body = val;
}

void Projectile::set_player(Player* val) {
	player = val;
}

void Projectile::set_damage(float val) {
	damage = val;
}

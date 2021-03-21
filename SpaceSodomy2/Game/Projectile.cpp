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

Counter* Projectile::get_hp() {
	return hp;
}

Damage_Receiver* Projectile::get_damage_receiver() {
	return damage_receiver;
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

void Projectile::set_hp(Counter* val) {
	hp = val;
}

void Projectile::set_damage_receiver(Damage_Receiver* val) {
	damage_receiver = val;
}

void Projectile::set_damage(float val) {
	damage = val;
}

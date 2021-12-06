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

DamageReceiver* Projectile::get_damage_receiver() {
	return damage_receiver;
}


Effects_Prototype* Projectile::get_effects_def() {
	return effects_prototype;
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

void Projectile::set_damage_receiver(DamageReceiver* val) {
	damage_receiver = val;
}

void Projectile::set_damage(float val) {
	damage = val;
}

void Projectile::set_effects_prototype(Effects_Prototype* val) {
	effects_prototype = val;
}
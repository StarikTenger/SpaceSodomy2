#include "pch.h"
#include "Ship.h"

Ship::Ship() {}

Ship::~Ship() {}

// Get methods
Player* Ship::get_player() {
	return player;
}

Engine* Ship::get_engine() {
	return engine;
}

b2Body* Ship::get_body() {
	return body;
}

Gun* Ship::get_gun() {
	return gun;
}

Counter* Ship::get_hp() {
	return hp;
}

Counter* Ship::get_stamina() {
	return stamina;
}

Damage_Receiver* Ship::get_damage_receiver() {
	return damage_receiver;
}

Effects* Ship::get_effects() {
	return effects;
}

Bonus_Slot* Ship::get_bonus_slot() {
	return bonus_slot;
}

// Set methods
void Ship::set_player(Player* val) {
	player = val;
}

void Ship::set_engine(Engine* val) {
	engine = val;
}

void Ship::set_body(b2Body* val) {
	body = val;
}

void Ship::set_gun(Gun* val) {
	gun = val;
}

void Ship::set_hp(Counter* val) {
	hp = val;
}

void Ship::set_stamina(Counter* val) {
	stamina = val;
}

void Ship::set_damage_receiver(Damage_Receiver* val) {
	damage_receiver = val;
}

void Ship::set_effects(Effects* val) {
	effects = val;
}

void Ship::set_bonus_slot(Bonus_Slot* val) {
	bonus_slot = val;
}

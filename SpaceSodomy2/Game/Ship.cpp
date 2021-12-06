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
Counter* Ship::get_energy() {
	return energy;
}
DamageReceiver* Ship::get_damage_receiver() {
	return damage_receiver;
}

Module* Ship::get_left_module() {
	return left_module;
}
Module* Ship::get_right_module() {
	return right_module;
}

Effects* Ship::get_effects() {
	return effects;
}

BonusSlot* Ship::get_bonus_slot() {
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
void Ship::set_energy(Counter* val) {
	energy = val;
}
void Ship::set_damage_receiver(DamageReceiver* val) {
	damage_receiver = val;
}

void Ship::set_effects(Effects* val) {
	effects = val;
}

void Ship::set_bonus_slot(BonusSlot* val) {
	bonus_slot = val;
}


void Ship::set_left_module(Module* val) {
	left_module = val;
}
void Ship::set_right_module(Module* val) {
	right_module = val;
}
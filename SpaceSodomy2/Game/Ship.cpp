#include "pch.h"
#include "Ship.h"

Ship::Ship() {}

Ship::~Ship() {}

// Get methods
Player* Ship::get_player() {
	return player;
}

Command_Module* Ship::get_command_module() {
	return command_module;
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

Damage_Receiver* Ship::get_damage_receiver() {
	return damage_receiver;
}

// Set methods
void Ship::set_player(Player* val) {
	player = val;
}

void Ship::set_command_module(Command_Module* val) {
	command_module = val;
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

void Ship::set_damage_receiver(Damage_Receiver* val) {
	damage_receiver = val;
}

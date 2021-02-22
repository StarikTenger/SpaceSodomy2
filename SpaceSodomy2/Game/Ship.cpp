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

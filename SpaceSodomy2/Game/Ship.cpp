#include "pch.h"
#include "Ship.h"

Ship::Ship() {}

Ship::~Ship() {}

// Get methods
int Ship::get_id() {
	return id;
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
void Ship::set_id(int val) {
	id = val;
}
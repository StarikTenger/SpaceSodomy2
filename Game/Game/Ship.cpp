#include "pch.h"
#include "Ship.h"

Ship::Ship() {}

Ship::~Ship() {}

// Get methods
Command_Module* Ship::get_command_module() {
	return command_module;
}
Engine* Ship::get_engine() {
	return engine;
}
b2Body* Ship::get_body() {
	return body;
}
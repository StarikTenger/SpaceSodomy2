#include "pch.h"
#include "CommandModule.h"
#include <iostream>

// Constructor
CommandModule::CommandModule() {}

std::vector<int> CommandModule::get_active() {
	return active;
}

// Get methods
int CommandModule::get_command(int id) {
	if (id < 0 || id >= active.size()) // vector out of range
		return 0;
	return active[id];
}

// Set methods
void CommandModule::set_command(int id, int value) {
	if (id < 0 || id >= active.size()) // vector out of range
		return;
	active[id] = value;
}

void CommandModule::reset() {
	for (auto& command : active)
		command = 0;
}

void CommandModule::set_rocket_angle(float val) {
	rocket_angle = val;
}
float CommandModule::get_rocket_angle() {
	return rocket_angle;
}

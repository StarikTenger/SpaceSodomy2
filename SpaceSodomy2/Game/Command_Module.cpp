#include "pch.h"
#include "Command_Module.h"

// Constructor
Command_Module::Command_Module() {}

std::vector<int> Command_Module::get_active() {
	return active;
}

// Get methods
int Command_Module::get_command(int id) {
	if (id < 0 || id >= active.size()) // vector out of range
		return 0;
	return active[id];
}

// Set methods
void Command_Module::set_command(int id, int value) {
	if (id < 0 || id >= active.size()) // vector out of range
		return;
	active[id] = value;
}

void Command_Module::reset() {
	for (auto& command : active)
		command = 0;
}

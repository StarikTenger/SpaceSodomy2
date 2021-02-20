#include "pch.h"
#include "Command_Module.h"

// Constructor
Command_Module::Command_Module() {}

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
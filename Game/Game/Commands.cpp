#include "pch.h"
#include "Commands.h"

// Constructor
Commands::Commands() {}

// Get methods
int Commands::get_command(int id) {
	if (id < 0 || id >= active.size()) // vector out of range
		return 0;
	return active[id];
}

// Set methods
void Commands::set_command(int id, int value) {
	if (id < 0 || id >= active.size()) // vector out of range
		return;
	active[id] = value;
}
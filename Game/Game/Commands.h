#pragma once
#include <vector>

class Commands {
private:
	std::vector<int> active; // Shows which commands are active

public:
	enum Command_Names {
		// Engines
		ENGINE_LIN_FORWARD,
		ENGINE_LIN_BACKWARD,
		ENGINE_LIN_LEFT,
		ENGINE_LIN_RIGHT,
		ENGINE_ANG_LEFT,
		ENGINE_ANG_RIGHT
	};
	// Constructor
	Commands();

	// Get methods
	int get_command(int id);

	// Set methods
	void set_command(int id, int value);
};


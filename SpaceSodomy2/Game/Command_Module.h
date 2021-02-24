#pragma once
#include <vector>

class Command_Module {
private:
	std::vector<int> active = std::vector<int>(COMMAND_COUNT, 0); // Shows which commands are active

public:
	enum Command_Names {
		// Engines
		ENGINE_LIN_FORWARD,
		ENGINE_LIN_BACKWARD,
		ENGINE_LIN_LEFT,
		ENGINE_LIN_RIGHT,
		ENGINE_ANG_LEFT,
		ENGINE_ANG_RIGHT,

		COMMAND_COUNT
	};
	// Constructor
	Command_Module();

	// Get methods
	std::vector<int> get_active();
	int get_command(int id);

	// Set methods
	void set_command(int id, int value);

	// Resets all commands to 0
	void reset();
};


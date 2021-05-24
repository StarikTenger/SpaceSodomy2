#pragma once
#include <vector>
#include <map>
#include "iId.h"

class Command_Module : public iId {
private:
	std::map<int, float> commands;
	static std::vector<int> ship_commands;
	static std::vector<int> rocket_commands;
public:
	enum Command_Names {
		// Engines
		ENGINE_LIN_FORWARD,
		ENGINE_LIN_BACKWARD,
		ENGINE_LIN_LEFT,
		ENGINE_LIN_RIGHT,
		ENGINE_ANG_LEFT,
		ENGINE_ANG_RIGHT,
		STABILIZE_ROTATION,
		SHOOT,
		BOOST,
		BONUS_ACTIVATION,
		RESPAWN,
		LEFT_MODULE,
		RIGHT_MODULE,
		ROCKET_ANGLE,
		COMMAND_COUNT
	};
	// Constructor
	Command_Module();

	// Get methods
	std::vector<int> get_active_for_ship();
	float get_command(int id);

	// Set methods
	void set_command(int id, int value);

	// Resets all commands to 0
	void reset();
};


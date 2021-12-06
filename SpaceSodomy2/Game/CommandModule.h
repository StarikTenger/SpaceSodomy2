#pragma once
#include <vector>
#include "iId.h"

class CommandModule : public iId {
private:
	std::vector<int> active = std::vector<int>(COMMAND_COUNT, 0); // Shows which commands are active
	float rocket_angle = 0;

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
	CommandModule();

	// Get methods
	std::vector<int> get_active();
	int get_command(int id);

	// Set methods
	void set_command(int id, int value);

	// Resets all commands to 0
	void reset();
	void set_rocket_angle(float);
	float get_rocket_angle();
};
#include "pch.h"
#include "Command_Module.h"

// Constructor
Command_Module::Command_Module() {}

std::vector<int> Command_Module::ship_commands = {
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
		RIGHT_MODULE
};

std::vector<int> Command_Module::rocket_commands = {
		ENGINE_LIN_FORWARD,
		ROCKET_ANGLE,

};

std::vector<int> Command_Module::get_active_for_ship() {
	std::vector<int> ans;
	for (int i = 0; i < ship_commands.size(); i++) {
		ans.push_back((int)commands[ship_commands[i]]);
	}
	return ans;
}

// Get methods
float Command_Module::get_command(int id) {
	if (!commands.count(id)) {
		return 0;
	}
	return commands[id];
}

// Set methods
void Command_Module::set_command(int id, int value) {
	commands[id] = value;
}

void Command_Module::reset() {
	for (auto& command : commands) {
		command.second = 0;
	}
}

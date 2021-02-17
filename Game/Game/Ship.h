#pragma once
#include <box2d/box2d.h>
#include "Engine.h"

class Ship {
private:
	int id = 0;

	// Entities
	Command_Module* command_module = nullptr;
	Engine* engine = nullptr;
	b2Body* body = nullptr;

public:
	Ship();
	~Ship();

	// Get methods
	Command_Module* get_command_module();
	Engine* get_engine();
	b2Body* get_body();
};


#pragma once
#include <box2d/box2d.h>
#include "Engine.h"
#include "Player.h"

class Ship {
private:
	// Entities
	Player* player = nullptr;
	Command_Module* command_module = nullptr;
	Engine* engine = nullptr;
	b2Body* body = nullptr;

public:
	Ship();
	~Ship();

	// Get methods
	Player* get_player();
	Command_Module* get_command_module();
	Engine* get_engine();
	b2Body* get_body();

	// Set methods
	void set_player(Player*);
	void set_command_module(Command_Module*);
	void set_engine(Engine*);
	void set_body(b2Body*);

};


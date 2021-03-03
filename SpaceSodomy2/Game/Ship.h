#pragma once
#include <box2d/box2d.h>
#include "Engine.h"
#include "Player.h"
#include "Gun.h"
#include "Counter.h"

class Ship {
private:
	// Entities
	Player* player = nullptr;
	Command_Module* command_module = nullptr;
	Engine* engine = nullptr;
	b2Body* body = nullptr;
	Gun* gun = nullptr;
	Counter* hp = nullptr;

public:
	Ship();
	~Ship();

	// Get methods
	Player* get_player();
	Command_Module* get_command_module();
	Engine* get_engine();
	b2Body* get_body();
	Gun* get_gun();
	Counter* get_hp();

	// Set methods
	void set_player(Player*);
	void set_command_module(Command_Module*);
	void set_engine(Engine*);
	void set_body(b2Body*);
	void set_gun(Gun*);
	void set_hp(Counter*);
};


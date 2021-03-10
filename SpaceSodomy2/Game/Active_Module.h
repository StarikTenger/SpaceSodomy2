#pragma once
#include <box2d/box2d.h>
#include "Player.h"
#include "Command_Module.h"
#include "Counter.h"
#include "iId.h"

class Active_Module : public iId {
protected:
	// Common for all modules
	float recharge_time = 1;
	// Physical body
	b2Body* body = nullptr;
	// Related player
	Player* player = nullptr;
	// Related command
	int bind = Command_Module::SHOOT;
	// Time left
	Counter* recharge_counter = nullptr;

public:
	Active_Module();
	void set(b2Body*, Player*);

	// Get methods
	float get_recharge_time();
	b2Body* get_body();

	// Set methods
	void set_recharge_time(float);
	void set_body(b2Body*);
	void set_recharge_counter(Counter*);
	void set_bind(int);

	// Activates module
	virtual void activate() = 0;
	// Func to call regularly
	void step(float dt);
};


#pragma once
#include <box2d/box2d.h>

class Active_Module {
protected:
	// Common for all modules
	static float recharge_time;
	// Physical body
	b2Body* body = nullptr;
	// Time left
	float time_to_recharge = recharge_time;

public:
	Active_Module();

	// Get methods
	static float get_recharge_time();
	b2Body* get_body();

	// Set methods
	static void set_recharge_time(float);
	void set_body(b2Body*);

	// Activates module
	int activate();
	// Func to call regularly
	void step(float dt);
};


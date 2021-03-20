#pragma once
#include <box2d/box2d.h>
#include "Event_Manager.h"
#include "Player.h"
#include "Command_Module.h"
#include "Counter.h"
#include "iId.h"

class Active_Module : public iId {
protected:
	// Common for all modules
	float recharge_time = 0.1;
	float stamina_consumption = 2;
	// Physical body
	b2Body* body = nullptr;
	// Related player
	Player* player = nullptr;
	// Related command
	int bind = Command_Module::SHOOT;
	// Time left
	Counter* recharge_counter = nullptr;
	Counter* stamina_cooldown_delay_counter = nullptr;
	// Sound manager
	Event_Manager* event_manager = nullptr;
	// Stamina
	Counter* stamina;

public:
	Active_Module();
	void set(b2Body*, Player*);

	// Get methods
	float get_recharge_time();
	b2Body* get_body();
	Counter* get_recharge_counter();
	Counter* get_stamina_cooldown_delay_counter();
	Event_Manager* get_event_manager();
	Counter* get_stamina();


	// Set methods
	void set_recharge_time(float);
	void set_body(b2Body*);
	void set_recharge_counter(Counter*);
	void set_bind(int);
	void set_event_manager(Event_Manager*);
	void set_stamina(Counter*);

	// Activates module
	virtual void activate() = 0;
	// Func to call regularly
	void step(float dt);
};


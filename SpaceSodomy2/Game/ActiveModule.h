#pragma once
#include <box2d/box2d.h>
#include "EventManager.h"
#include "Player.h"
#include "CommandModule.h"
#include "Counter.h"
#include "iId.h"
#include "Effects.h"

class ActiveModule : public iId {
protected:
	float recharge_time = 10;
	float stamina_cost = 20;
	float energy_cost = 0;
	float stamina_delay = 0.7;
	// Physical body
	b2Body* body = nullptr;
	// Related player
	Player* player = nullptr;
	// Related command
	int bind = CommandModule::SHOOT;
	// Time left
	Counter* recharge_counter = nullptr;
	//Counter* stamina_cooldown_delay_counter = nullptr;  // WTF is this
	// Event manager
	EventManager* event_manager = nullptr;
	// Stamina
	Counter* stamina;
	Counter* energy;
	// Dispensable effects
	Effects_Prototype* effects_prototype = nullptr;
	// Effects affecting this
	Effects* effects = nullptr;
	

public:
	ActiveModule();
	virtual ~ActiveModule() = default;
	void set(b2Body*, Player*);

	// Get methods
	float get_recharge_time();
	b2Body* get_body();
	Counter* get_recharge_counter();
	//Counter* get_stamina_cooldown_delay_counter();
	EventManager* get_event_manager();
	Counter* get_stamina();
	Counter* get_energy();
	Effects_Prototype* get_effects();

	// Set methods
	void set_recharge_time(float);
	void set_body(b2Body*);
	void set_recharge_counter(Counter*);
	void set_bind(int);
	void set_event_manager(EventManager*);
	void set_stamina(Counter*);
	void set_energy(Counter*);
	void set_effects_prototype(Effects_Prototype*);
	void set_effects(Effects*);


	// Activates module
	virtual void activate() = 0;
	// Side effects of activation
	void activate_default_side_effects();
	// Func to call regularly
	void step(float dt);
};


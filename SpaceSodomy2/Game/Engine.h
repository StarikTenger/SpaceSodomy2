#pragma once
#include <vector>
#include <box2d/box2d.h>
#include "CommandModule.h"
#include "Counter.h"
#include "iId.h"
#include "Effects.h"

class Engine : public iId{
private:
	float dt = 0;
	// Properties
	float force_linear = 1.5;
	float force_angular = 2;
	float current_modifier = 2;
	float boost_modifier = 2;
	float boost_stamina_consumption = 20;

	// To manage boost correctly
	bool is_linear_force_used = 0;

	// Command interface
	CommandModule* command_module = nullptr;
	// Physical body
	b2Body* body = nullptr;
	// Stamina
	Counter* stamina = nullptr;
	// Effects
	Effects* effects;

	// Forces' methods
	void apply_force_linear(b2Vec2 direction);
	void apply_force_angular(float dir);
	void stabilize_rotation();
public:
	// Constructor
	Engine();
	Engine(b2Body*, CommandModule*, Counter*, Effects*);
	// Get methods
	float get_force_linear();
	float get_force_angular();
	// Set methods
	void set_force_linear(float value);
	void set_force_angular(float value);
	// Step
	void step(float _dt);
};


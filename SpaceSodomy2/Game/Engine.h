#pragma once
#include <vector>
#include <box2d/box2d.h>
#include "Command_Module.h"

class Engine {
private:
	// Properties
	float force_linear = 1;
	float force_angular = 1;

	// Command interface
	Command_Module* command_module = nullptr;
	// Physical body
	b2Body* body = nullptr;

	// Forces' methods
	void apply_force_linear(b2Vec2 direction);
	void apply_force_angular(float dir);
public:
	// Constructor
	Engine();
	Engine(b2Body*, Command_Module*);
	// Get methods
	float get_force_linear();
	float get_force_angular();
	// Set methods
	void set_force_linear(float value);
	void set_force_angular(float value);
	// Step
	void step();
};


#pragma once
#include <vector>
#include <box2d/box2d.h>

class Engine {
private:
	// Properties
	float force_linear = 1;
	float force_angular = 1;
	// Command interface

	// Physical body
	b2Body* body;
public:
	// Constructor
	Engine();
	// Get methods
	float get_force_linear();
	float get_force_angular();
	// Set methods
	void set_force_linear(float value);
	void set_force_angular(float value);
};


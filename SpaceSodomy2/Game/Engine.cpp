#include "pch.h"
#include "Engine.h"
#include <iostream>

// Constructor
Engine::Engine() {}

Engine::Engine(b2Body* _body, Command_Module* _command_module) {
	body = _body;
	command_module = _command_module;
}

// Forces' methods
void Engine::apply_force_linear(b2Vec2 direction) {
	// Rotation matrix
	b2Transform transform(b2Vec2_zero, b2Rot(body->GetAngle()));
	// Apply rotation matrix
	direction = b2Mul(transform, direction);
	// Apply force in correct direction
	body->ApplyForceToCenter(force_linear * direction, 1);
}

void Engine::apply_force_angular(float dir) {
	body->ApplyTorque(dir * force_angular, 1);
}

// Get methods
float Engine::get_force_linear() {
	return force_linear;
}
float Engine::get_force_angular() {
	return force_angular;
}

// Set methods
void Engine::set_force_linear(float value) {
	force_linear = value;
}
void Engine::set_force_angular(float value) {
	force_angular = value;
}

// Step
void Engine::step() {
	// Command processing
	if (command_module->get_command(Command_Module::ENGINE_LIN_FORWARD))
		apply_force_linear(b2Vec2(1, 0));
	if (command_module->get_command(Command_Module::ENGINE_LIN_BACKWARD))
		apply_force_linear(b2Vec2(-1, 0));
	if (command_module->get_command(Command_Module::ENGINE_LIN_LEFT))
		apply_force_linear(b2Vec2(0, -1));
	if (command_module->get_command(Command_Module::ENGINE_LIN_RIGHT))
		apply_force_linear(b2Vec2(0, 1));
	if (command_module->get_command(Command_Module::ENGINE_ANG_LEFT))
		apply_force_angular(-1);
	if (command_module->get_command(Command_Module::ENGINE_ANG_RIGHT))
		apply_force_angular(1);
}
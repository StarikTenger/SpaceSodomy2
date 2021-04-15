#include "pch.h"
#include "Engine.h"
#include <iostream>

// Constructor
Engine::Engine() {}

Engine::Engine(b2Body* _body, Command_Module* _command_module, Counter* _stamina, Effects* _effects) {
	body = _body;
	command_module = _command_module;
	stamina = _stamina;
	effects = _effects;
}

// Forces' methods
void Engine::apply_force_linear(b2Vec2 direction) {
	is_linear_force_used = 1;
	// Rotation matrix
	b2Transform transform(b2Vec2_zero, b2Rot(body->GetAngle()));
	// Apply rotation matrix
	direction = b2Mul(transform, direction);
	// Apply force in correct direction
	body->ApplyForceToCenter(force_linear * current_modifier * direction, 1);
}

void Engine::apply_force_angular(float dir) {
	body->ApplyTorque(dir * force_angular * body->GetFixtureList()->GetShape()->m_radius, 1);
}

void Engine::stabilize_rotation() {
	if (command_module->get_command(Command_Module::ENGINE_ANG_LEFT) ||
		command_module->get_command(Command_Module::ENGINE_ANG_RIGHT))
		return;
	float eps = 0.5;
	if (body->GetAngularVelocity() < -eps)
		command_module->set_command(Command_Module::ENGINE_ANG_RIGHT, 1);
	else if (body->GetAngularVelocity() > eps)
		command_module->set_command(Command_Module::ENGINE_ANG_LEFT, 1);
	else
		body->SetAngularVelocity(0);
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
void Engine::step(float _dt) {
	dt = _dt;
	// Boost management
	current_modifier = 1;
	if (stamina->get() > 0 && command_module->get_command(Command_Module::BOOST)) {
		current_modifier *= boost_modifier;
	}
	if (effects->get_effect(Effects::Types::CHARGE)->get_counter()->get() > 0) {     // Apply CHARGE
		current_modifier *= effects->get_effect(Effects::Types::CHARGE)->get_strength();
	}
	is_linear_force_used = 0;
	// Command processing
	if (command_module->get_command(Command_Module::STABILIZE_ROTATION))
		stabilize_rotation();
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
	// Boost consumption
	if (stamina->get() > 0 && is_linear_force_used && command_module->get_command(Command_Module::BOOST)) {
		stamina->modify(-dt * boost_stamina_consumption);
	}
}
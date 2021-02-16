#include "pch.h"
#include "Engine.h"

// Constructor
Engine::Engine() {}
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
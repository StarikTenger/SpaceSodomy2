#pragma once
struct HullPrototype {
	float hp = 100;
	float mass = 1;
	unsigned char alias = 1;
	float radius = 0.4;
	float stamina = 100;
	float stamina_recovery = 50;
	float force_linear = 1.5;
	float force_angular = 2;
	float stamina_pause = 0.7;
	float energy = 100;
	float energy_regen = 0;
	float start_energy = 50;
};
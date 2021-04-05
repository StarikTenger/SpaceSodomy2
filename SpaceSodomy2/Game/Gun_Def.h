#pragma once
#include "Effects.h"

struct Gun_Def {
	float damage = 20;
	float recharge_time = 0.5;
	float stamina_consumption = 10;
	float projectile_mass = 0.05;
	float projectile_vel = 10;
	float projectile_radius = 0.2;
	float projectile_hp = 20;
	Effects_Def effect_def;
};


#pragma once
#include <box2d/box2d.h>
#include "Player.h"
#include "Effects.h"

// Projectile definition
struct Projectile_Def {
	Combatant* player = nullptr;
	b2Vec2 pos;
	b2Vec2 vel;
	float angle = 0.f;
	float damage = 0.f;
	float mass = 0.05f;
	float radius = 0.2f;
	float hp = 20.f;
	Effects_Prototype* effects_prototype  = nullptr;

	Projectile_Def();
};


#pragma once
#include <box2d/box2d.h>
#include "Player.h"
#include "Effects.h"

// Projectile definition
struct Projectile_Def {
	Player* player = nullptr;
	b2Vec2 pos;
	b2Vec2 vel;
	float angle = 0;
	float damage = 0;
	float mass = 0.05;
	float radius = 0.2;
	float hp = 20;
	Effects_Prototype* effects_prototype  = nullptr;

	Projectile_Def();
};


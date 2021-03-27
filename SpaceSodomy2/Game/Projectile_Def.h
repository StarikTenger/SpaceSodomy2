#pragma once
#include <box2d/box2d.h>
#include "Player.h"

// Projectile defenition
struct Projectile_Def {
	Player* player = nullptr;
	b2Vec2 pos;
	b2Vec2 vel;
	float angle = 0;
	float damage = 0;
	float mass = 0.05;
	float radius = 0.2;

	Projectile_Def();
};


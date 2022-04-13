#pragma once
#include <set>
#include <box2d/box2d.h>


class Ship;
class Engine;
class ActiveModule;
class Projectile;
class DamageReceiver;
class Bonus;
class Rocket;
class Forcefield;
class Wall;


// Pavlik Morosov class for Game
// Contains only physical objects (exclude Player)
class GameReadable {
public:
    std::set<Ship*> ships;
	std::set<ActiveModule*> active_modules;

	std::set<Projectile*> projectiles;
	std::set<DamageReceiver*> damage_receivers;
	std::set<Bonus*> bonuses;
	std::set<Rocket*> rockets;
	std::set<Forcefield*> forcefields;

	// Walls
	std::set<Wall*> walls;

	b2World physics = b2World(b2Vec2_zero);
};
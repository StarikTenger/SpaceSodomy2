#pragma once
#include <set>
#include <box2d/box2d.h>
#include <functional>


class Ship;
class Engine;
class ActiveModule;
class Projectile;
class DamageReceiver;
class Bonus;
class Rocket;
class Forcefield;
class Wall;
class Engine;
class CommandModule;
class Counter;
class Event;
class Effects;


// Pavlik Morosov class for Game
// Contains only physical objects (exclude Player)
class GameReadable {
public:
	float dt = 0.02;
	float time = 0;


    std::set<Ship*> ships;
	std::set<Engine*> engines;
	std::set<CommandModule*> command_modules;

	std::set<ActiveModule*> active_modules;
	std::set<Projectile*> projectiles;
	std::set<Counter*> counters;
	std::set<DamageReceiver*> damage_receivers;
	std::set<Event*> events;
	std::set<Effects*> effects;

	std::set<Bonus*> bonuses;
	std::set<Rocket*> rockets;
	std::set<Forcefield*> forcefields;

	// Walls
	std::set<Wall*> walls;

	b2World* physics = new b2World(b2Vec2_zero);

	std::function<b2Vec2(b2Vec2, float)> calc_intersection_;
};
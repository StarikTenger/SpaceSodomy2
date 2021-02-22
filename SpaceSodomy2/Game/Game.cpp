// Game.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "Game.h"

Game::Game() {
	create_ship(b2Vec2(0, 0), 0, 0);
}

b2Body* Game::create_round_body(b2Vec2 pos, float angle, float radius, float mass) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
	bodyDef.gravityScale = 1;

	b2CircleShape circle;
	circle.m_p.Set(0, 0);
	circle.m_radius = radius;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circle;
	fixtureDef.density = mass / (b2_pi * radius * radius);
	fixtureDef.friction = 0;

	bodyDef.linearVelocity = b2Vec2_zero;
	b2Body* body = physics.CreateBody(&bodyDef);
	body->SetSleepingAllowed(0);
	body->CreateFixture(&fixtureDef);
	return body;

}

Ship* Game::create_ship(b2Vec2 pos, float angle, int id) {
	// Creating body
	auto body = create_round_body(pos, angle, 0.4, 1);

	auto ship = new Ship();
	auto command_module = new Command_Module();
	auto engine = new Engine(body, command_module);

	// Matching modules to ship
	ship->set_command_module(command_module);
	ship->set_engine(engine);
	ship->set_body(body);

	ships.push_back(ship);
	engines.push_back(engine);
	command_modules.insert({id, command_module });
	return ship;
}

void Game::process_engines() {
	for (auto engine : engines) {
		engine->step();
	}
}

void Game::apply_command(int id, int command, int val) {
	command_modules[id]->set_command(command, val);
}

void Game::step(float dt) {
	process_engines();
	physics.Step(dt, 10, 10);
}

void Game::clear() {
	// Clear ships
	for (auto ship : ships)
		delete ship;
	ships = {};
	// Clear players
	for (auto player : players)
		delete player;
	ships = {};
	// Clear command_modules
	for (auto command_module : command_modules)
		delete command_module.second;
	command_modules = {};
	// Clear engines
	for (auto engine : engines)
		delete engine;
	engines = {};
	// Clear physics
	b2World physics = b2World(b2Vec2_zero);
}

std::string Game::encode() {
	// Encoding ships
	std::string message = "";
	for (auto ship : ships) {
		message += "S ";
		// Id (wait for player class)
		message += std::to_string(ship->get_player()->get_id()) + " ";
		// Pos
		message += std::to_string(ship->get_body()->GetPosition().x) + " ";
		message += std::to_string(ship->get_body()->GetPosition().y) + " ";
		// Angle
		message += std::to_string(ship->get_body()->GetAngle()) + " ";
	}
	return message;
}

void Game::decode(std::string source) {
	// First clear
	clear();

	// Creating stringstream
	std::stringstream stream;
	stream << source;

	std::string symbol;
	while (stream >> symbol) {
		// Ship
		if (symbol == "S") {
			int id;
			stream >> id;
			b2Vec2 pos;
			stream >> pos.x >> pos.y;
			float angle;
			stream >> angle;
			Ship* ship = create_ship(pos, angle, id);
		}
	}

}


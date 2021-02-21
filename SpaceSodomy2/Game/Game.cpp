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

Ship* Game::create_ship(b2Vec2 pos, float angle, int team) {
	// Creating body
	auto body = create_round_body(pos, angle, 0.4, 1);

	auto ship = new Ship();
	auto command_module = new Command_Module();
	auto engine = new Engine(body, command_module);

	ships.push_back(ship);
	engines.push_back(engine);
	command_modules.insert({team, command_module });
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

std::string Game::encode() {
	// TODO: write encoder here
	return "biba"; // Temporary solution
}

void Game::decode(std::string source) {
	// TODO: write decoder here
}

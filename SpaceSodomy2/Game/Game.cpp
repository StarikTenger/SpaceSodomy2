// Game.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include <iostream>
#include "framework.h"
#include "Game.h"

Game::Game() {
	create_player(0, {255, 0, 0}, "biba", b2Vec2(0, 0), 0);
}

b2Body* Game::create_round_body(b2Vec2 pos, float angle, float radius, float mass) {
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = pos;
	bodyDef.angle = angle;
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
	body->GetFixtureList()->SetFriction(0);
	body->GetFixtureList()->SetRestitutionThreshold(0);
	body->GetFixtureList()->SetRestitution(0.5);
	return body;
}

Ship* Game::create_ship(Player* player, b2Vec2 pos, float angle) {
	// Creating body
	auto body = create_round_body(pos, angle, 0.4, 1);

	auto ship = new Ship();
	auto command_module = new Command_Module();
	auto engine = new Engine(body, command_module);

	// Matching modules to ship
	ship->set_player(player);
	ship->set_command_module(command_module);
	ship->set_engine(engine);
	ship->set_body(body);

	ships.push_back(ship);
	engines.push_back(engine);
	command_modules.insert({player->get_id(), command_module });
	return ship;
}

Wall* Game::create_wall(std::vector<b2Vec2> vertices) {
	Wall* wall = new Wall();
	wall->set(&physics, vertices);
	walls.push_back(wall);
	return wall;
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
	players = {};
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

int Game::load_walls(std::string path) {
	map_path = path;
	std::ifstream input(path);
	//std::stringstream input;
	// Parsing
	std::string symbol;
	while (input >> symbol) {
		if (symbol == "END")
			break;
		// Wall
		if (symbol == "WALL") {
			std::string symbol_1;
			std::vector<b2Vec2> points;
;			while (input >> symbol_1) {
				if (symbol_1 == "END")
					break;
				if (symbol_1 == "POINT") {
					b2Vec2 point;
					if (!(input >> point.x >> point.y)) { // Error
						std::cerr << "Game::load_walls: failed to read point";
						return false;
					}
					// Point loaded successfully
					points.push_back(point);
					continue;
				}
				std::cerr << "Game::load_walls: unknown symbol " << symbol_1 << "\n";
				return false;
			}
			// Wall loaded successfully
			create_wall(points);
			continue;
		}
		std::cerr << "Game::load_walls: unknown symbol " << symbol << "\n";
		return false;
	}
	return true;
}

std::string Game::encode() {
	// Encoding ships
	std::string message = "";
	// Map path
	message += "M " + map_path + " ";
	// Ships
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
	std::cout << source << "\n";

	// Creating stringstream
	std::stringstream stream;
	stream << source;

	std::string symbol;
	while (stream >> symbol) {
		// Map
		if (symbol == "M") {
			std::string path;
			stream >> path;
			if (map_path != path) {
				map_path = path;
				load_walls(map_path);
			}
		}
		// Ship
		if (symbol == "S") {
			int id;
			stream >> id;
			b2Vec2 pos;
			stream >> pos.x >> pos.y;
			float angle;
			stream >> angle;
			create_player(id, {255, 0, 0}, "_name_", pos, angle);
		}
	}

}

void Game::create_player(int id, sf::Color color, std::string name, b2Vec2 pos, float angle) {
	std::cout << "New Player\n";
	Player* player = new Player(id, color, name);
	players.push_back(player);
	create_ship(player, pos, angle);
}

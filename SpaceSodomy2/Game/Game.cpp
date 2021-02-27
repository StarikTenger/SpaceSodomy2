// Game.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include <iostream>
#include "framework.h"
#include "Game.h"

Game::Game() {
	create_player(0, {255, 0, 0}, "biba", b2Vec2(0, 0), 0);
	// Contact filter
	physics.SetContactFilter(&collision_filter);
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
	auto gun = new Gun();
	gun->set(body, player, command_module);
	gun->set_projectile_manager(&projectile_manager);

	// Matching entities to ship
	ship->set_player(player);
	ship->set_command_module(command_module);
	ship->set_engine(engine);
	ship->set_body(body);
	ship->set_gun(gun);

	ships.push_back(ship);
	engines.push_back(engine);
	command_modules.insert({player->get_id(), command_module });
	active_modules.push_back(gun);
	return ship;
}

Wall* Game::create_wall(std::vector<b2Vec2> vertices, int orientation, float restitution) {
	Wall* wall = new Wall();
	wall->set(&physics, vertices, orientation);
	wall->get_body()->GetFixtureList()->SetRestitution(restitution);
	collision_filter.add_body(wall->get_body(), Collision_Filter::WALL);
	walls.push_back(wall);
	return wall;
}

Projectile* Game::create_projectile(Projectile_Def projectile_def) {
	// Creating body
	auto body = create_round_body(projectile_def.pos, projectile_def.angle, 0.2, 0.1);
	body->SetLinearVelocity(projectile_def.vel);
	collision_filter.add_body(body, Collision_Filter::PROJECTILE);

	// Creating projectile
	auto projectile = new Projectile();
	projectile->set_body(body);
	projectile->set_player(projectile_def.player);
	projectile->set_damage(projectile_def.damage);

	// Adding to vectors
	projectiles.push_back(projectile);

	return projectile;
}

void Game::process_engines() {
	for (auto engine : engines)
		engine->step();
}

void Game::process_active_modules() {
	for (auto active_module : active_modules)
		active_module->step(dt);
}

void Game::process_projectlie_manager() {
	Projectile_Def projectile_def;
	while (projectile_manager.get_next(projectile_def)) {
		create_projectile(projectile_def);
	}
}

void Game::apply_command(int id, int command, int val) {
	command_modules[id]->set_command(command, val);
}

void Game::step(float _dt) {
	dt = _dt;
	process_engines();
	process_active_modules();
	process_projectlie_manager();

	// Physics
	for (b2Contact* contact = physics.GetContactList(); contact; contact = contact->GetNext())
		contact->SetRestitution(contact->GetFixtureA()->GetRestitution() * 
			contact->GetFixtureB()->GetRestitution());
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
	// Clear projectiles
	for (auto projectile : projectiles)
		delete projectile;
	projectiles = {};
	// Clear physics
	b2World physics = b2World(b2Vec2_zero);
}

int Game::load_map(std::string path) {
	map_path = path;
	std::ifstream file_input(path);
	std::stringstream input = aux::comment(file_input);
	// Parsing
	std::string symbol;
	while (input >> symbol) {
		if (symbol == "END")
			break;
		// Wall
		if (symbol == "WALL") {
			std::string symbol_1;
			std::vector<b2Vec2> points;
			int orientation = Wall::OUTER;
			float restitution = 0.5;
;			while (input >> symbol_1) {
				if (symbol_1 == "END")
					break;
				if (symbol_1 == "POINT") {
					b2Vec2 point;
					if (!(input >> point.x >> point.y)) { // Error
						std::cerr << "Game::load_map: failed to read point";
						return false;
					}
					// Point loaded successfully
					points.push_back(point);
					continue;
				}
				if (symbol_1 == "INNER") {
					orientation = Wall::INNER;
					continue;
				}
				if (symbol_1 == "OUTER") {
					orientation = Wall::OUTER;
					continue;
				}
				if (symbol_1 == "RESTITUTION") {
					if (!(input >> restitution)) {
						std::cerr << "Game::load_map: failed to read restitution";
						return false;
					}
					continue;
				}
				std::cerr << "Game::load_map: unknown symbol " << symbol_1 << "\n";
				return false;
			}
			// Wall loaded successfully
			create_wall(points, orientation, restitution);
			continue;
		}
		std::cerr << "Game::load_map: unknown symbol " << symbol << "\n";
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
		// Id
		message += std::to_string(ship->get_player()->get_id()) + " ";
		// Pos
		message += std::to_string(ship->get_body()->GetPosition().x) + " ";
		message += std::to_string(ship->get_body()->GetPosition().y) + " ";
		// Angle
		message += std::to_string(ship->get_body()->GetAngle()) + " ";
		// Commands
		message += aux::mask_to_string(ship->get_command_module()->get_active()) + " ";
	}

	// Projectiles
	for (auto projectile : projectiles) {
		message += "P ";
		// Id
		message += std::to_string(projectile->get_player()->get_id()) + " ";
		// Pos
		message += std::to_string(projectile->get_body()->GetPosition().x) + " ";
		message += std::to_string(projectile->get_body()->GetPosition().y) + " ";
		// Angle
		message += std::to_string(projectile->get_body()->GetAngle()) + " ";
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
		// Map
		if (symbol == "M") {
			std::string path;
			stream >> path;
			if (map_path != path) {
				map_path = path;
				load_map(map_path);
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
			std::string commands_stringed;
			stream >> commands_stringed;
			std::vector<int> commands = aux::string_to_mask(commands_stringed);
			auto ship = create_player(id, {255, 0, 0}, "_name_", pos, angle);
			for (int i = 0; i < commands.size(); i++)
				ship->get_command_module()->set_command(i, commands[i]);
		}
		// Projectile
		if (symbol == "P") {
			int id;
			stream >> id;
			b2Vec2 pos;
			stream >> pos.x >> pos.y;
			float angle;
			stream >> angle;

			Projectile_Def projectile_def;
			projectile_def.pos = pos;

			auto projectile = create_projectile(projectile_def);
		}
	}

}

Ship* Game::create_player(int id, sf::Color color, std::string name, b2Vec2 pos, float angle) {
	Player* player = new Player(id, color, name);
	players.push_back(player);
	return create_ship(player, pos, angle);
}

void Game::del_player(int id) {
	for (auto ship : ships) {
		if (ship->get_player()->get_id() == id) {
			for (auto it = players.begin(); it != players.end(); it++)
				if (ship->get_player() == *it) {
					players.erase(it);
					break;
				}
			delete ship->get_player();
			for (auto it = command_modules.begin(); it != command_modules.end(); it++)
				if (ship->get_command_module() == it->second) {
					command_modules.erase(it);
					break;
				}
			delete ship->get_command_module();
			for (auto it = engines.begin(); it != engines.end(); it++)
				if (ship->get_engine() == *it) {
					engines.erase(it);
					break;
				}
			delete ship->get_engine();
			for (auto it = ships.begin(); it != ships.end(); it++)
				if (ship == *it) {
					ships.erase(it);
					break;
				}
			delete ship;
		}
	}
}
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

Gun* Game::create_gun() {
	auto gun = new Gun();
	active_modules.insert({ id_manager.set_id(gun), gun });
	// Counter
	auto counter = create_counter();
	counter->set_change_vel(-1);
	gun->set_recharge_counter(counter);

	return gun;
}

Command_Module* Game::create_command_module() {
	auto command_module = new Command_Module();
	command_modules.insert({ id_manager.set_id(command_module), command_module });
	return command_module;
}

Engine* Game::create_engine(b2Body* body, Command_Module* command_module) {
	auto engine = new Engine(body, command_module);
	engines.insert({ id_manager.set_id(engine), engine });
	return engine;
}

Counter* Game::create_counter(float val, float change_vel) {
	auto counter = new Counter();
	counter->set(val);
	counter->set_change_vel(change_vel);
	counters.insert({ id_manager.set_id(counter) ,counter });
	return counter;
}

Damage_Receiver* Game::create_damage_receiver(b2Body* body, Counter* hp) {
	auto damage_receiver = new Damage_Receiver(body, hp);
	damage_receivers.insert({ id_manager.set_id(damage_receiver), damage_receiver });
	return damage_receiver;
}

Ship* Game::create_ship(Player* player, b2Vec2 pos, float angle) {
	// Creating ship
	auto ship = new Ship();
	id_manager.set_id(ship);
	ships.insert({ ship->get_id(), ship });

	// Player
	ship->set_player(player);

	// Body
	auto body = create_round_body(pos, angle, 0.4, 1);
	collision_filter.add_body(body, Collision_Filter::STANDART, ship->get_player()->get_id());
	ship->set_body(body);

	// Command module
	auto command_module = new Command_Module();
	player->set_command_module(command_module);

	// Engine
	auto engine = create_engine(body, command_module);
	ship->set_engine(engine);

	// Gun
	auto gun = create_gun();
	gun->set(body, player);
	gun->set_projectile_manager(&projectile_manager);
	ship->set_gun(gun);

	// Hp
	auto hp = create_counter(100);
	ship->set_hp(hp);

	// Damage receiver
	auto damage_receiver = create_damage_receiver(body, hp);
	ship->set_damage_receiver(damage_receiver);

	return ship;
}

Wall* Game::create_wall(std::vector<b2Vec2> vertices, int orientation, float restitution) {
	Wall* wall = new Wall();
	wall->set(&physics, vertices, orientation);
	wall->get_body()->GetFixtureList()->SetRestitution(restitution);
	collision_filter.add_body(wall->get_body(), Collision_Filter::WALL);
	walls.insert({ id_manager.set_id(wall), wall });
	return wall;
}

Projectile* Game::create_projectile(Projectile_Def projectile_def) {
	// Creating body
	auto body = create_round_body(projectile_def.pos, projectile_def.angle, 0.2, 0.1);
	body->SetLinearVelocity(projectile_def.vel);
	collision_filter.add_body(body, Collision_Filter::PROJECTILE, projectile_def.player->get_id());

	// Creating projectile
	auto projectile = new Projectile();
	projectile->set_body(body);
	projectile->set_player(projectile_def.player);
	projectile->set_damage(projectile_def.damage);

	// Adding to vectors
	projectiles.insert({ id_manager.set_id(projectile), projectile });

	return projectile;
}

void Game::delete_body(b2Body* body) {
	physics.DestroyBody(body);
}

void Game::delete_projectile(Projectile* projectile) {
	delete_body(projectile->get_body());
	projectiles.erase(projectile->get_id());
	delete projectile;
}

void Game::delete_engine(Engine* engine) {
	engines.erase(engine->get_id());
	delete engine;
}

void Game::delete_active_module(Active_Module* active_module) {
	active_modules.erase(active_module->get_id());
	delete active_module;
}

void Game::delete_ship(Ship* ship) {
	delete_body(ship->get_body());
	delete_engine(ship->get_engine());
	delete_active_module(ship->get_gun());
	ships.erase(ship->get_id());
	delete ship;
}

void Game::delete_damage_receiver(Damage_Receiver* damage_receiver) {
	damage_receivers.erase(damage_receiver->get_id());
	delete damage_receiver;
}

void Game::process_engines() {
	for (auto engine : engines)
		engine.second->step();
}

void Game::process_projectiles() {
	std::set<Projectile*> projectiles_to_delete;
	// Dealing damage
	for (auto projectile : projectiles) {
		for (auto damage_receiver : damage_receivers) {
			if (contact_table.check(projectile.second->get_body(),
				damage_receiver.second->get_body())) {
				std::cout << "damage\n";
				damage_receiver.second->damage(projectile.second->get_damage());
				projectiles_to_delete.insert(projectile.second);
			}
		}
	}

	// Deleting
	for (auto projectile : projectiles) {
		// Checking for wall collision
		for (auto wall : walls) {
			if (contact_table.check(projectile.second->get_body(), wall.second->get_body()))
				projectiles_to_delete.insert(projectile.second);
		}
	}
	for (auto projectile : projectiles_to_delete)
		delete_projectile(projectile);
}

void Game::process_active_modules() {
	for (auto active_module : active_modules)
		active_module.second->step(dt);
}

void Game::process_projectlie_manager() {
	Projectile_Def projectile_def;
	while (projectile_manager.get_next(projectile_def)) {
		create_projectile(projectile_def);
	}
}

void Game::process_physics() {
	contact_table.reset();
	for (b2Contact* contact = physics.GetContactList(); contact; contact = contact->GetNext()) {
		if (contact->IsTouching())
			contact_table.add(contact->GetFixtureA()->GetBody(), 
				contact->GetFixtureB()->GetBody());
		contact->SetRestitution(contact->GetFixtureA()->GetRestitution() *
			contact->GetFixtureB()->GetRestitution());
	}
	physics.Step(dt, 10, 10);
}

void Game::process_counters() {
	for (auto counter : counters)
		counter.second->step(dt);
}

void Game::apply_command(int id, int command, int val) {
	players[id]->get_command_module()->set_command(command, val);
}

void Game::step(float _dt) {
	dt = _dt;
	process_physics();
	process_engines();
	process_projectiles();
	process_active_modules();
	process_projectlie_manager();
	process_counters();
}

void Game::clear() {
	// Clear ships
	for (auto ship : ships)
		delete ship.second;
	ships = {};
	// Clear players
	for (auto player : players)
		delete player.second;
	players = {};
	// Clear command_modules
	for (auto command_module : command_modules)
		delete command_module.second;
	command_modules = {};
	// Clear engines
	for (auto engine : engines)
		delete engine.second;
	engines = {};
	// Clear projectiles
	for (auto projectile : projectiles)
		delete projectile.second;
	projectiles = {};
	// Clear counters
	for (auto counter : counters)
		delete counter.second;
	counters = {};
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
		message += std::to_string(ship.second->get_id()) + " ";
		// Player id
		message += std::to_string(ship.second->get_player()->get_id()) + " ";
		// Pos
		message += std::to_string(ship.second->get_body()->GetPosition().x) + " ";
		message += std::to_string(ship.second->get_body()->GetPosition().y) + " ";
		// Angle
		message += std::to_string(ship.second->get_body()->GetAngle()) + " ";
		// Commands
		message += aux::mask_to_string(ship.second->get_player()->get_command_module()->get_active()) + " ";
	}

	// Projectiles
	for (auto projectile : projectiles) {
		message += "P ";
		// Id
		message += std::to_string(projectile.second->get_id()) + " ";
		// Player id
		message += std::to_string(projectile.second->get_player()->get_id()) + " ";
		// Pos
		message += std::to_string(projectile.second->get_body()->GetPosition().x) + " ";
		message += std::to_string(projectile.second->get_body()->GetPosition().y) + " ";
		// Angle
		message += std::to_string(projectile.second->get_body()->GetAngle()) + " ";
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
			int id, player_id;
			stream >> id >> player_id;
			b2Vec2 pos;
			stream >> pos.x >> pos.y;
			float angle;
			stream >> angle;
			std::string commands_stringed;
			stream >> commands_stringed;
			std::vector<int> commands = aux::string_to_mask(commands_stringed);
			auto ship = create_player(player_id, {255, 0, 0}, "_name_", pos, angle);
			for (int i = 0; i < commands.size(); i++)
				ship->get_player()->get_command_module()->set_command(i, commands[i]);
		}
		// Projectile
		if (symbol == "P") {
			int id, player_id;
			stream >> id >> player_id;
			b2Vec2 pos;
			stream >> pos.x >> pos.y;
			float angle;
			stream >> angle;

			Projectile_Def projectile_def;
			projectile_def.pos = pos;
			projectile_def.player = players[player_id];

			auto projectile = create_projectile(projectile_def);
		}
	}

}

Ship* Game::create_player(int id, sf::Color color, std::string name, b2Vec2 pos, float angle) {
	Player* player = new Player(id, color, name);
	players[id] = player;
	auto ship = create_ship(player, pos, angle);
	player->set_command_module(ship->get_player()->get_command_module());
	return ship;
}

void Game::delete_player(int id) {
	// Deleting ship
	std::deque<Ship*> ships_to_delete;

	for (auto ship : ships)
		if (ship.second->get_player() == players[id])
			ships_to_delete.push_back(ship.second);

	for (auto ship : ships_to_delete)
		delete_ship(ship);

	// Deleting player
	players.erase(players.find(id));
}
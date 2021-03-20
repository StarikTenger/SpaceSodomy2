// Game.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include <iostream>
#include "framework.h"
#include "Game.h"

Game::Game() {
	new_player(0, {255, 0, 0}, "biba", b2Vec2(0, 0), 0);
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
	active_modules.insert(gun);
	// Counter
	auto counter = create_counter();
	auto cooldown_delay = create_counter();
	counter->set_change_vel(-1);
	cooldown_delay->set_change_vel(-1);
	gun->set_recharge_counter(counter);
	gun->set_stamina_cooldown_delay_counter(cooldown_delay);
	// Managers
	gun->set_projectile_manager(&projectile_manager);
	gun->set_event_manager(&sound_manager);
	// Id
	id_manager.set_id(gun);
	return gun;
}

Player* Game::create_player(int id, sf::Color color, std::string name) {
	auto player = new Player();
	player->set_color(color);
	player->set_name(name);
	player->set_command_module(create_command_module());
	player->set_time_to_respawn(create_counter(0, -1));
	// Id
	player->set_id(id);
	players[player->get_id()] = player;
	return player;
}

Command_Module* Game::create_command_module() {
	auto command_module = new Command_Module();
	command_modules.insert(command_module);
	id_manager.set_id(command_module);
	return command_module;
}

Engine* Game::create_engine(b2Body* body, Command_Module* command_module) {
	auto engine = new Engine(body, command_module);
	engines.insert(engine);
	id_manager.set_id(engine);
	return engine;
}

Counter* Game::create_counter(float val, float change_vel) {
	auto counter = new Counter();
	counter->set(val);
	counter->set_change_vel(change_vel);
	counters.insert(counter);
	id_manager.set_id(counter);
	return counter;
}

Damage_Receiver* Game::create_damage_receiver(b2Body* body, Counter* hp) {
	auto damage_receiver = new Damage_Receiver(body, hp);
	damage_receivers.insert(damage_receiver);
	id_manager.set_id(damage_receiver);
	return damage_receiver;
}

Ship* Game::create_ship(Player* player, b2Vec2 pos, float angle) {
	// Creating ship
	auto ship = new Ship();
	ships.insert(ship);
	id_manager.set_id(ship);

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
	ship->set_gun(gun);

	// Hp
	auto hp = create_counter(100);
	ship->set_hp(hp);

	// Stamina
	auto stamina = create_counter(100);
	gun->set_stamina(stamina);
	ship->set_stamina(stamina);

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
	walls.insert(wall);
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
	projectiles.insert(projectile);
	id_manager.set_id(projectile);

	return projectile;
}

Sound* Game::create_event(std::string name, b2Body* body, float playing_offset) {
	auto sound = new Sound();
	sound->set_body(body);
	sound->set_name(name);
	sound->set_playing_offset(create_counter(playing_offset, 1));
	id_manager.set_id(sound);
	sounds.insert(sound);
	return sound;
}

void Game::delete_body(b2Body* body) {
	physics.DestroyBody(body);
	collision_filter.delete_body(body);
}

void Game::delete_projectile(Projectile* projectile) {
	delete_body(projectile->get_body());
	projectiles.erase(projectile);
	delete projectile;
}

void Game::delete_engine(Engine* engine) {
	engines.erase(engine);
	delete engine;
}

void Game::delete_active_module(Active_Module* active_module) {
	active_modules.erase(active_module);
	delete_counter(active_module->get_recharge_counter());
	delete active_module;
}

void Game::delete_ship(Ship* ship) {
	delete_body(ship->get_body());
	delete_engine(ship->get_engine());
	delete_active_module(ship->get_gun());
	delete_counter(ship->get_hp());
	delete_counter(ship->get_stamina());
	// Player management
	ship->get_player()->set_is_alive(0);
	ships.erase(ship);
	delete ship;
}

void Game::delete_damage_receiver(Damage_Receiver* damage_receiver) {
	damage_receivers.erase(damage_receiver);
	delete damage_receiver;
}

void Game::delete_counter(Counter* counter) {
	counters.erase(counter);
	delete counter;
}

void Game::delete_sound(Sound* sound) {
	delete_counter(sound->get_playing_offset());
	sounds.erase(sound);
	delete sound;
}

void Game::process_players() {
	// Creating ships
	for (auto player_pair : players) {
		auto player = player_pair.second;
		if (!player->get_is_alive() && player->get_time_to_respawn()->get() < 0) {
			// TODO: function to determine new ship position
			player->set_is_alive(1);
			create_ship(player, {0, 0}, 0);
		}
	}
}

void Game::process_ships() {
	// Deleting
	std::set<Ship*> ships_to_delete;
	for (auto ship : ships) {
		// Checking for < zero hp
		if (ship->get_hp()->get() <= 0)
			ships_to_delete.insert(ship);
	}
	for (auto ship : ships_to_delete)
		delete_ship(ship);
}

void Game::process_engines() {
	for (auto engine : engines)
		engine->step();
}

void Game::process_projectiles() {
	std::set<Projectile*> projectiles_to_delete;
	// Dealing damage
	for (auto projectile : projectiles) {
		for (auto damage_receiver : damage_receivers) {
			if (contact_table.check(projectile->get_body(),
				damage_receiver->get_body())) {
				std::cout << "damage\n";
				damage_receiver->damage(projectile->get_damage());
				projectiles_to_delete.insert(projectile);
			}
		}
	}

	// Deleting
	for (auto projectile : projectiles) {
		// Checking for wall collision
		for (auto wall : walls) {
			if (contact_table.check(projectile->get_body(), wall->get_body()))
				projectiles_to_delete.insert(projectile);
		}
	}
	for (auto projectile : projectiles_to_delete)
		delete_projectile(projectile);
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

void Game::process_sound_manager() {
	Event_Def event_def;
	while (sound_manager.get_next(event_def)) {
		create_event(event_def.name, event_def.body);
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
		counter->step(dt);
}

void Game::process_sounds() {
	std::set<Sound*> sounds_to_delete;
	for (auto sound : sounds) {
		if (!sound->is_alive())
			sounds_to_delete.insert(sound);
	}
	for (auto sound : sounds_to_delete)
		delete_sound(sound);
}

void Game::apply_command(int id, int command, int val) {
	players[id]->get_command_module()->set_command(command, val);
}

void Game::step(float _dt) {
	dt = _dt;
	process_physics();
	process_players();
	process_ships();
	process_engines();
	process_projectiles();
	process_active_modules();
	process_projectlie_manager();
	process_sound_manager();
	process_counters();
	process_sounds();
}

void Game::clear() {
	// Clear ships
	for (auto ship : ships)
		delete ship;
	ships = {};
	// Clear players
	for (auto player : players)
		delete player.second;
	players = {};
	// Clear command_modules
	for (auto command_module : command_modules)
		delete command_module;
	command_modules = {};
	// Clear engines
	for (auto engine : engines)
		delete engine;
	engines = {};
	// Clear projectiles
	for (auto projectile : projectiles)
		delete projectile;
	projectiles = {};
	// Clear counters
	for (auto counter : counters)
		delete counter;
	counters = {};
	// Clear sounds
	for (auto sound : sounds)
		delete sound;
	sounds = {};
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

	// Players
	for (auto player : players) {
		message += "P ";
		// Id
		message += std::to_string(player.first) + " ";
		// Color
		message += std::to_string(player.second->get_color().r) + " ";
		message += std::to_string(player.second->get_color().g) + " ";
		message += std::to_string(player.second->get_color().b) + " ";
		// Name
		message += player.second->get_name() + " ";
	}

	// Ships
	for (auto ship : ships) {
		message += "S ";
		// Id
		message += std::to_string(ship->get_id()) + " ";
		// Player id
		message += std::to_string(ship->get_player()->get_id()) + " ";
		// Pos
		message += std::to_string(ship->get_body()->GetPosition().x) + " ";
		message += std::to_string(ship->get_body()->GetPosition().y) + " ";
		// Angle
		message += std::to_string(ship->get_body()->GetAngle()) + " ";
		// Commands
		message += aux::mask_to_string(ship->get_player()->get_command_module()->get_active()) + " ";
		// Hp
		message += std::to_string(ship->get_hp()->get()) + " ";
		// Stamina
		message += std::to_string(ship->get_stamina()->get()) + " ";
	}

	// Projectiles
	for (auto projectile : projectiles) {
		message += "p ";
		// Id
		message += std::to_string(projectile->get_id()) + " ";
		// Player id
		message += std::to_string(projectile->get_player()->get_id()) + " ";
		// Pos
		message += std::to_string(projectile->get_body()->GetPosition().x) + " ";
		message += std::to_string(projectile->get_body()->GetPosition().y) + " ";
		// Angle
		message += std::to_string(projectile->get_body()->GetAngle()) + " ";
	}

	// Events
	for (auto sound : sounds) {
		message += "e ";
		// Id
		message += std::to_string(sound->get_id()) + " ";
		// Name
		message += sound->get_name() + " ";
		// Pos
		message += std::to_string(sound->get_body()->GetPosition().x) + " ";
		message += std::to_string(sound->get_body()->GetPosition().y) + " ";
	}

	return message;
}

Ship* Game::new_player(int id, sf::Color color, std::string name, b2Vec2 pos, float angle) {
	Player* player = create_player(id, color, name);
	players[id] = player;
	auto ship = create_ship(player, pos, angle);
	player->set_command_module(ship->get_player()->get_command_module());
	return ship;
}

void Game::delete_player(int id) {
	// Deleting ship
	std::deque<Ship*> ships_to_delete;

	for (auto ship : ships)
		if (ship->get_player() == players[id])
			ships_to_delete.push_back(ship);

	for (auto ship : ships_to_delete)
		delete_ship(ship);

	// Deleting player
	players.erase(players.find(id));
}
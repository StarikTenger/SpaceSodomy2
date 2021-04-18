// Game.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include <iostream>
#include "framework.h"
#include "Game.h"

b2Vec2 Game::get_rand_respawn_pos() {
	bool acceptable = 0;
	b2Vec2 respawn_point;
	while (!acceptable) {
		acceptable = 1;
		respawn_point = b2Vec2(aux::random_int(lower_left_corner.x, upper_right_corner.x) + aux::random_float(-1, 1, 4),
			aux::random_int(lower_left_corner.y, upper_right_corner.y) + aux::random_float(-1, 1, 4));
		//respawn_point = b2Vec2(0, 0);
		for (auto wall : walls) {
			acceptable = !aux::is_in_polygon(respawn_point, wall->get_vertices(),
				wall->get_orientation());
			if (!acceptable)
				break;
		}
	}
	return respawn_point;
}

Game::Game() {
	// Contact filter
	//physics.SetContactFilter(&collision_filter);
	physics.SetContactListener(&contact_table);
	contact_table.set_collision_filter(&collision_filter);
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

Gun* Game::create_gun(Gun_Prototype def) {
	auto gun = new Gun();
	active_modules.insert(gun);
	// Counter
	auto counter = create_counter();
	counter->set_change_vel(-1);
	gun->set_recharge_counter(counter);
	// Managers
	gun->set_projectile_manager(&projectile_manager);
	gun->set_event_manager(&event_manager);
	// Characteristics
	gun->import_Gun_Prototype(def);
	gun->set_effects_prototype(&def.effect_prototype);
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

Engine* Game::create_engine(b2Body* body, Command_Module* command_module, Counter* stamina, Effects* effs) {
	auto engine = new Engine(body, command_module, stamina, effs);
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

Damage_Receiver* Game::create_damage_receiver(b2Body* body, Counter* hp, Player* player, Effects* effs) {
	auto damage_receiver = new Damage_Receiver(body, hp);
	damage_receiver->set_player(player);
	damage_receiver->set_effects(effs);
	damage_receivers.insert(damage_receiver);
	id_manager.set_id(damage_receiver);
	return damage_receiver;
}

Ship* Game::create_ship(Player* player, b2Vec2 pos, float angle) {
	// Creating ship
	auto ship = new Ship();
	ships.insert(ship);
	id_manager.set_id(ship);

	// Create effects
	Effects_Prototype effects_prototype;
	for (int i = 0; i < Effects::Types::COUNT; i++) {
		effects_prototype.effects[i].set_type(effect_params.effects[i].get_type());
		effects_prototype.effects[i].set_strength(effect_params.effects[i].get_strength());
	}
	auto effs = create_effects(&effects_prototype);
	ship->set_effects(effs);

	// Bonus slot
	ship->set_bonus_slot(create_bonus_slot());
	ship->get_bonus_slot()->set_effects(effs);

	// Player
	ship->set_player(player);

	// Hull def
	Hull_Prototype hull_prototype;
	if (hulls.count(player->get_hull_name()))
		hull_prototype = hulls[player->get_hull_name()];

	// Body
	auto body = create_round_body(pos, angle, hull_prototype.radius, hull_prototype.mass);
	collision_filter.add_body(body, Collision_Filter::STANDART, ship->get_player()->get_id());
	ship->set_body(body);

	// Command module
	auto command_module = new Command_Module();
	player->set_command_module(command_module);

	// Gun
	Gun_Prototype gun_prototype;
	if (guns.count(player->get_gun_name())) {
		gun_prototype = guns[player->get_gun_name()];

	}
	auto gun = create_gun(gun_prototype);
	gun->set(body, player);
	gun->set_effects_prototype(nullptr);
	if (guns.count(player->get_gun_name())) {
		gun->set_effects_prototype(&guns[player->get_gun_name()].effect_prototype);
	}
	gun->set_ship_effects(effs);
	ship->set_gun(gun);

	// Hp
	auto hp = create_counter(hull_prototype.hp);
	hp->set_max(hull_prototype.hp);
	ship->set_hp(hp);

	// Stamina
	auto stamina = create_counter(hull_prototype.stamina, hull_prototype.stamina_recovery);
	stamina->set_max(hull_prototype.stamina);
	stamina->set_delay(0.7);
	gun->set_stamina(stamina);
	ship->set_stamina(stamina);

	// Engine
	auto engine = create_engine(body, command_module, stamina, effs);
	ship->set_engine(engine);

	// Damage receiver
	auto damage_receiver = create_damage_receiver(body, hp, player, effs);
	ship->set_damage_receiver(damage_receiver);


	return ship;
}

Wall* Game::create_wall(std::vector<b2Vec2> vertices, int orientation, float restitution) {
	Wall* wall = new Wall();
	wall->set(&physics, vertices, orientation);
	wall->get_body()->GetFixtureList()->SetRestitution(restitution);
	collision_filter.add_body(wall->get_body(), Collision_Filter::WALL);
	walls.insert(wall);
	id_manager.set_id(wall);
	return wall;
}

Projectile* Game::create_projectile(Projectile_Def projectile_def) {
	// Creating body
	auto body = create_round_body(projectile_def.pos, projectile_def.angle, projectile_def.radius, projectile_def.mass);
	body->SetLinearVelocity(projectile_def.vel);
	collision_filter.add_body(body, Collision_Filter::PROJECTILE, projectile_def.player->get_id());

	// Creating projectile
	auto projectile = new Projectile();
	projectile->set_body(body);
	projectile->set_player(projectile_def.player);
	projectile->set_damage(projectile_def.damage);
	projectile->set_hp(create_counter(projectile_def.hp, 0));
	projectile->set_damage_receiver(create_damage_receiver(body, projectile->get_hp(), projectile->get_player(), nullptr));
	projectile->set_effects_prototype(projectile_def.effects_prototype);

	// Adding to vectors
	projectiles.insert(projectile);
	id_manager.set_id(projectile);

	return projectile;
}

Sound* Game::create_event(Event_Def event_def, float playing_offset) {
	auto sound = new Sound();
	sound->set_body(event_def.body);
	sound->set_name(event_def.name);
	sound->set_pos(event_def.pos);
	sound->set_playing_offset(create_counter(playing_offset, 1));
	id_manager.set_id(sound);
	sounds.insert(sound);
	return sound;
}

Effects* Game::create_effects(Effects_Prototype* val) {
	auto _effects = new Effects(val);
	effects.insert(_effects);
	return _effects;
}

Bonus* Game::create_bonus(Bonus_Def val) {
	auto ans = new Bonus;
	auto body = create_round_body(val.pos, 0, bonus_manager.get_prototype(val.type)->radius, 1);
	collision_filter.add_body(body, Collision_Filter::PROJECTILE, 0);
	ans->set_id(val.get_id());
	ans->set_body(body);
	ans->set_type(val.type);
	bonuses.insert(ans);
	return ans;
}

Bonus_Slot* Game::create_bonus_slot() {
	auto ans = new Bonus_Slot;
	ans->set_bonus_manager(&bonus_manager);
	return ans;
}

void Game::delete_body(b2Body* body) {
	physics.DestroyBody(body);
	collision_filter.delete_body(body);
}

void Game::delete_projectile(Projectile* projectile) {
	delete_body(projectile->get_body());
	delete_counter(projectile->get_hp());
	delete_damage_receiver(projectile->get_damage_receiver());
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
	delete_damage_receiver(ship->get_damage_receiver());
	delete_counter(ship->get_hp());
	delete_counter(ship->get_stamina());
	delete_effects(ship->get_effects());
	delete_bonus_slot(ship->get_bonus_slot());
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

void Game::delete_effects(Effects* _effects) {
	effects.erase(_effects);
	delete _effects;
}

void Game::delete_bonus(Bonus* bonus) {
	bonuses.erase(bonus);
	delete_body(bonus->get_body());
	bonus_manager.free_bonus_spawn(bonus->get_type(), bonus->get_id());
	delete bonus;
}

void Game::delete_bonus_slot(Bonus_Slot* bonus_slot) {
	delete bonus_slot;
}

void Game::process_players() {
	// Creating ships
	for (auto player_pair : players) {
		auto player = player_pair.second;
		if (!player->get_is_alive() && player->get_time_to_respawn()->get() < 0 &&
			player->get_command_module()->get_command(Command_Module::RESPAWN)) {
			player->set_is_alive(1);

			// creating ship
			auto ship = create_ship(player, get_rand_respawn_pos(), aux::random_float(0, 2 * b2_pi, 3));
			//auto_damage = 0;
		}
	}
}

void Game::process_ships() {
	// Deleting
	std::set<Ship*> ships_to_delete;
	for (auto ship : ships) {
		if (auto_damage)
			ship->get_hp()->modify(-dt*20);

		// Apply INSTANT_HP
		auto hp_eff = ship->get_effects()->get_effect(Effects::Types::INSTANT_HP)->get_counter();
		if (hp_eff->get() > 0) {
			ship->get_hp()->modify(hp_eff->get());
			hp_eff->set(0);

		}
		// Apply INSTANT_STAMINA
		auto st_eff = ship->get_effects()->get_effect(Effects::Types::INSTANT_STAMINA)->get_counter();
		if (st_eff->get() > 0) {
			ship->get_stamina()->modify(st_eff->get());
			st_eff->set(0);

		}
		// Apply LASER_BURN
		if (ship->get_effects()->get_effect(Effects::Types::LASER_BURN)->get_counter()->get() > 0) {
			ship->get_damage_receiver()->damage(dt * ship->get_effects()->get_effect(Effects::Types::LASER_BURN)->get_strength(), 
				                                ship->get_damage_receiver()->get_last_hit());
		}

		// Apply laser
		if (ship->get_effects()->get_effect(Effects::Types::LASER)->get_counter()->get() > 0) {
			for (auto damage_receiver : damage_receivers) {
				if (ship->get_player()->get_id() == damage_receiver->get_player()->get_id())
					continue;
				float angle = ship->get_body()->GetAngle();
				b2Vec2 pos = ship->get_body()->GetPosition();
				b2Vec2 target_pos = damage_receiver->get_body()->GetPosition();
				b2Vec2 intersection = get_beam_intersection(pos, angle);
				if (aux::dist_from_segment(target_pos, pos, intersection) < 
					ship->get_body()->GetFixtureList()->GetShape()->m_radius + 0.1) {
					damage_receiver->damage(dt * 1000, ship->get_player());
					//damage_receiver->get_effects()->get_effect(Effects::LASER_BURN)->get_counter()->set(0.5);
				}
			}
		}

		// Bonus activating
		if (ship->get_player()->get_command_module()->get_command(Command_Module::BONUS_ACTIVATION)) {
			if (ship->get_bonus_slot()) {
				if (ship->get_bonus_slot()->get_current_bonus() == Bonus::LASER)
					event_manager.create_event(Event_Def("laser", ship->get_body()));
				ship->get_bonus_slot()->activate();				
			}
		}

		// Checking for < zero hp
		if (ship->get_hp()->get() <= 0) {
			ships_to_delete.insert(ship);
			ship->get_player()->add_death();
			if (ship->get_damage_receiver()->get_last_hit() != nullptr)
				ship->get_damage_receiver()->get_last_hit()->add_kill();
		}
		if (ship->get_effects()->get_effect(Effects::Types::CHARGE)->get_counter()->get() > 0) { // Apply CHARGE
			for (auto damage_receiver : damage_receivers) {
				if (contact_table.check(ship->get_body(), damage_receiver->get_body()) &&
					ship->get_player()->get_id() != damage_receiver->get_player()->get_id()) {
					damage_receiver->damage(0, ship->get_player());            // TODO
					if (damage_receiver->get_effects()) {
						damage_receiver->get_effects()->get_effect(Effects::Types::LASER_BURN)->get_counter()->set(0.5);
					}
				}
			}
		}
	}
	for (auto ship : ships_to_delete)
		delete_ship(ship);
}

void Game::process_engines() {
	for (auto engine : engines)
		engine->step(dt);
}

void Game::process_projectiles() {
	std::set<Projectile*> projectiles_to_delete;
	// Dealing damage
	for (auto projectile : projectiles) {
		// Dealing damage & applying effects
		for (auto damage_receiver : damage_receivers) {
			if (contact_table.check(projectile->get_body(), damage_receiver->get_body()) &&
				projectile->get_player()->get_id() != damage_receiver->get_player()->get_id()) {
				damage_receiver->damage(projectile->get_damage(), projectile->get_player());
				if (damage_receiver->get_effects() && projectile->get_effects_def()) {
					damage_receiver->get_effects()->update(projectile->get_effects_def());
				}
			}
		}
		// Checking for wall collision
		for (auto wall : walls) {
			if (contact_table.check(projectile->get_body(), wall->get_body()))
				projectiles_to_delete.insert(projectile);
		}
		// Checking for ship collision
		for (auto ship : ships) {
			if (projectile->get_player()->get_id() != ship->get_player()->get_id() &&
				contact_table.check(projectile->get_body(), ship->get_body()))
				projectiles_to_delete.insert(projectile);
		}
		// Checking hp
		if (projectile->get_hp()->get() <= 0) {
			projectiles_to_delete.insert(projectile);
		}
	}

	// Deleting
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
	while (event_manager.get_next(event_def)) {
		create_event(event_def);
	}
}

void Game::process_physics() {
	auto contact_table_prev = contact_table;
	contact_table.reset();
	for (b2Contact* contact = physics.GetContactList(); contact; contact = contact->GetNext()) {
		contact->SetRestitution(contact->GetFixtureA()->GetRestitution() *
			contact->GetFixtureB()->GetRestitution());
	}
	physics.Step(dt, 10, 10);
	for (b2Contact* contact = physics.GetContactList(); contact; contact = contact->GetNext()) {
		// Hit sound
		if (contact_table.check(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody()) &&
			!contact_table_prev.check(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody()) &&
			collision_filter.ShouldCollide(contact->GetFixtureA(), contact->GetFixtureB())) {
			event_manager.create_event(Event_Def("hit", nullptr, contact->GetManifold()->localPoint));
			std::cout << "hit\n";
		}
	}
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

void Game::process_effects() {
	for (auto eff : effects) {
		eff->step(dt);
	}
}

b2Vec2 Game::get_beam_intersection(b2Vec2 start, float angle) {
	b2Vec2 closest_intersection;
	float closest_distance = 1e9;
	b2Vec2 finish = start + 1e3 * aux::angle_to_vec(angle);
	for (auto wall : walls) {
		auto polygon = wall->get_vertices();
		for (int i = 0; i < polygon.size(); i++) {
			int j = (i + 1) % polygon.size();
			auto intersection = aux::segment_intersection({ start, finish }, {polygon[i], polygon[j]});
			if (intersection.first) {
				float distance = b2Distance(start, intersection.second);
				if (distance < closest_distance) {
					closest_distance = distance;
					closest_intersection = intersection.second;
				}
			}
		}
	}
	return closest_intersection;
}

void Game::process_bonuses() {
	std::deque<Bonus*> bonuses_to_delete;
	for (auto bonus : bonuses) {
		for (auto ship : ships) {
			if (contact_table.check(bonus->get_body(), ship->get_body())) {
				ship->get_bonus_slot()->add_bonus(bonus->get_type());
				bonuses_to_delete.push_back(bonus);
			}
		}
	}
	for (auto bonus : bonuses_to_delete)
		delete_bonus(bonus);
}
void Game::process_bonus_manager() {
	bonus_manager.step(dt);
	Bonus_Def def;
	while (bonus_manager.get_next(def)) {
		create_bonus(def);
	}
}

void Game::apply_command(int id, int command, int val) {
	players[id]->get_command_module()->set_command(command, val);
}

void Game::step(float _dt) {
	dt = _dt;
	time += dt;
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
	process_effects();
	process_bonuses();
	process_bonus_manager();
}

void Game::set_dt(float _dt) {
	dt = _dt;
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
	// Clear bonuses
	for (auto bonus : bonuses)
		delete bonus;
	bonuses = {};
	for (auto effect : effects) {
		delete effect;
	}
	effects = {};

	// Clear physics
	b2World physics = b2World(b2Vec2_zero);
}

bool Game::load_map(std::string path) {
	walls.clear();
	map_path = path;
	std::ifstream file_input(path);
	std::stringstream input = aux::comment(file_input);
	int wall_id = 0;
	bool corner_init = 0;

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
			create_wall(points, orientation, restitution)->set_id(wall_id);

			if (orientation == Wall::INNER) {
				for (auto point : points) {
					if (!corner_init) {
						lower_left_corner = point;
						upper_right_corner = point;
						corner_init = 1;
					}
					else {
						lower_left_corner.x = std::min(
							lower_left_corner.x,
							point.x);
						lower_left_corner.y = std::min(
							lower_left_corner.y,
							point.y);
						upper_right_corner.x = std::max(
							upper_right_corner.x,
							point.x);
						upper_right_corner.y = std::max(
							upper_right_corner.y,
							point.y);
					}
				}
			}

			wall_id++;
			continue;
		}
		if (symbol == "BONUS") {
			input >> symbol;
			auto type = Bonus::get_bonus_type(symbol);
			while (input >> symbol) {
				if (symbol == "END") {
					break;
				}
				if (symbol == "POINT") {
					b2Vec2 to_append;
					input >> to_append.x >> to_append.y;
					bonus_manager.add_spawnpoint(type, to_append);
					continue;
				}
				if (symbol == "COOLDOWN") {
					float temp;
					input >> temp;
					bonus_manager.set_cooldown(type, temp);
				}
				if (symbol == "MAX_COUNT") {
					int temp;
					input >> temp;
					bonus_manager.set_max_count(type, temp);
				}
			}
			continue;
		}
		std::cerr << "Game::load_map: unknown symbol " << symbol << "\n";
		return false;
	}
	return true;
}

bool Game::load_parameters(std::string path) {
	std::ifstream file_input(path);
	std::stringstream input = aux::comment(file_input);

	// Parsing
	std::string symbol;
	while (input >> symbol) {
		if (symbol == "END")
			break;


		// Lambdas for reading symbols
		auto read_symbol = [&](std::string symbol_name, float& var) {
			if (symbol == symbol_name) {
				float val;
				if (!(input >> val)) {
					std::cerr << "Game::load_parameters: failed to read " + symbol_name + "\n";
					std::cout << "Game::load_parameters: failed to read " + symbol_name + "\n";

					return false;
				}
				var = val;
			}
			return true;
		};
		auto read_symbol_bool = [&](std::string symbol_name, bool& var) {
			if (symbol == symbol_name) {
				bool val;
				if (!(input >> val)) {
					std::cerr << "Game::load_parameters: failed to read " + symbol_name + "\n";
					std::cout << "Game::load_parameters: failed to read " + symbol_name + "\n";

					return false;
				}
				var = val;
			}
			return true;
		};


		auto read_effect_prototype = [&]() {
			auto _effects = effect_params;
			while (input >> symbol) {
				if (symbol == "END") {
					break;
				}
				int type_ = Effects::get_effect_type(symbol);
				float temp;
				input >> temp;
				_effects.effects[type_].get_counter()->set(temp);
			}
			return _effects;
		};
		if (symbol == "EFFECT_TYPES") {
			while (input >> symbol) {
				if (symbol == "END")
					break;
				int type_ = Effects::get_effect_type(symbol);
				effect_params.effects[type_].set_type(Effects::Algebraic_Type::ADDITIVE);
				effect_params.effects[type_].set_strength(0);
				while (input >> symbol) {
					if (symbol == "END")
						break;
					if (symbol == "ALGEBRA_TYPE") {
						std::string temp;
						input >> temp;
						if (temp == "MAXIMAL") {
							effect_params.effects[type_].set_type(Effects::Algebraic_Type::MAXIMAL);
						}
						else if (temp == "ADDITIVE") {
							effect_params.effects[type_].set_type(Effects::Algebraic_Type::ADDITIVE);
						}
						else if (temp == "ANNULATOR") {
							effect_params.effects[type_].set_type(Effects::Algebraic_Type::ANNULATOR);
						}
					}
					if (symbol == "STRENGTH") {
						float temp;
						input >> temp;
						effect_params.effects[type_].set_strength(temp);
					}
				}
			}
			continue;
		}
		// Bonuses
		if (symbol == "BONUS") {
			std::string name;
			if (!(input >> name)) {
				std::cerr << "Game::load_parameters: failed to load BONUS name\n";
			}
			Bonus_Prototype bonus_prototype;
			bonus_prototype.effects_prototype = effect_params;
			bonus_prototype.type = Bonus::get_bonus_type(name);
			while (input >> symbol) {
				if (symbol == "END")
					break;
				if (symbol == "EFFECTS") {
					bonus_prototype.effects_prototype = read_effect_prototype();

				}
				read_symbol_bool("IS_INSTANT", bonus_prototype.is_instant);
				read_symbol("RADIUS", bonus_prototype.radius);
				float strength = 1;
				float cd = 0;
				read_symbol("COOLDOWN", cd);
				if (cd != 0) {
					bonus_manager.set_cooldown(bonus_prototype.type, cd);
				}
			}
			bonus_manager.add_prototype(bonus_prototype);
			continue;
		}
		// Gun
		if (symbol == "GUN") {
			std::string name;
			if (!(input >> name)) {
				std::cerr << "Game::load_parameters: failed to read GUN name\n";
				return false;
			}
			guns[name] = {};
			guns[name].effect_prototype = effect_params;
			while (input >> symbol) {
				if (symbol == "END")
					break;
				if (symbol == "EFFECTS") {
					guns[name].effect_prototype = read_effect_prototype();
				}
				read_symbol("RECHARGE", guns[name].recharge_time);
				read_symbol("DAMAGE", guns[name].damage);
				read_symbol("STAMINA_CONSUMPTION", guns[name].stamina_consumption);
				read_symbol("PROJECTILE_MASS", guns[name].projectile_mass);
				read_symbol("PROJECTILE_VEL", guns[name].projectile_vel);
				read_symbol("PROJECTILE_RADIUS", guns[name].projectile_radius);
				read_symbol("PROJECTILE_HP", guns[name].projectile_hp);
			}
			continue;
		}
		// Body
		if (symbol == "HULL") {
			std::string symbol_1;
			std::string name;
			if (!(input >> name)) {
				std::cerr << "Game::load_parameters: failed to read BODY name";
				return false;
			}
			hulls[name] = {};
			while (input >> symbol) {
				if (symbol == "END")
					break;
				
				read_symbol("HP", hulls[name].hp);
				read_symbol("MASS", hulls[name].mass);
				read_symbol("RADIUS", hulls[name].radius);
				read_symbol("STAMINA", hulls[name].stamina);
				read_symbol("STAMINA_RECOVERY", hulls[name].stamina_recovery);
			}
			continue;
		}
		std::cerr << "Game::load_parameters: unknown symbol " << symbol << "\n";
		return false;
	}
	return true;
}

std::string Game::encode() {
	std::string message = "";

	// Map path
	message += "M " + map_path + " ";

	// Players (P)
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
		// Hull
		message += player.second->get_hull_name() + " ";
		// Deaths & kills
		message += std::to_string(player.second->get_deaths()) + " ";
		message += std::to_string(player.second->get_kills()) + " ";
		// Time to respawn
		message += std::to_string(int(player.second->get_time_to_respawn()->get() + 0.99)) + " ";
		// Is alive
		message += std::to_string(int(player.second->get_is_alive())) + " ";
	}

	// Ships (S)
	for (auto ship : ships) {
		message += "S ";
		// Id
		message += std::to_string(ship->get_id()) + " ";
		// Player id
		message += std::to_string(ship->get_player()->get_id()) + " ";
		// Pos
		message += aux::float_to_string(ship->get_body()->GetPosition().x, 2) + " ";
		message += aux::float_to_string(ship->get_body()->GetPosition().y, 2) + " ";
		// Linear velocity
		message += aux::float_to_string(ship->get_body()->GetLinearVelocity().x, 3) + " ";
		message += aux::float_to_string(ship->get_body()->GetLinearVelocity().y, 3) + " ";
		// Angle
		message += aux::float_to_string(ship->get_body()->GetAngle(), 3) + " ";
		// Radius
		message += aux::float_to_string(ship->get_body()->GetFixtureList()->GetShape()->m_radius, 2) + " ";
		// Commands
		message += aux::mask_to_string(ship->get_player()->get_command_module()->get_active()) + " ";
		// Effects
		message += aux::mask_to_string(ship->get_effects()->get_mask()) + " ";
		// Bonus slot
		message += std::to_string(ship->get_bonus_slot()->get_current_bonus()) + " ";
		// Hp
		message += std::to_string((int)ship->get_hp()->get()) + " ";
		message += std::to_string((int)ship->get_hp()->get_max()) + " ";
		// Stamina
		message += std::to_string((int)ship->get_stamina()->get()) + " ";
		message += std::to_string((int)ship->get_stamina()->get_max()) + " ";
	}

	// Projectiles (p)
	for (auto projectile : projectiles) {
		message += "p ";
		// Id
		message += std::to_string(projectile->get_id()) + " ";
		// Player id
		message += std::to_string(projectile->get_player()->get_id()) + " ";
		// Pos
		message += aux::float_to_string(projectile->get_body()->GetPosition().x, 2) + " ";
		message += aux::float_to_string(projectile->get_body()->GetPosition().y, 2) + " ";
		// Angle
		message += aux::float_to_string(projectile->get_body()->GetAngle(), 3) + " ";
		// Radius
		message += aux::float_to_string(projectile->get_body()->GetFixtureList()->GetShape()->m_radius, 2) + " ";
	}

	// Bonuses (b)
	for (auto bonus : bonuses) {
		message += "b ";
		// Id
		message += std::to_string(bonus->get_id()) + " ";
		// Position
		message += aux::float_to_string(bonus->get_body()->GetPosition().x, 2) + " ";
		message += aux::float_to_string(bonus->get_body()->GetPosition().y, 2) + " ";
		// Type
		message += std::to_string(bonus->get_type()) + " ";
	}

	// Events (e)
	for (auto sound : sounds) {
		message += "e ";
		// Id
		message += std::to_string(sound->get_id()) + " ";
		// Name
		message += sound->get_name() + " ";
		// Pos
		message += aux::float_to_string(sound->get_pos().x, 2) + " ";
		message += aux::float_to_string(sound->get_pos().y, 2) + " ";
	}

	return message;
}

void Game::new_player(int id, sf::Color color, std::string name, std::string gun_name, std::string hull_name) {
	Player* player = create_player(id, color, name);
	player->set_gun_name(gun_name);
	player->set_hull_name(hull_name);
	players[id] = player;
	player->set_is_alive(0);
	player->get_time_to_respawn()->set(0);
}

Player* Game::player_by_id(int id) {
	if (!players.count(id))
		return nullptr;
	return players[id];
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
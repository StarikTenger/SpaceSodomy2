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
		acceptable = true;
		respawn_point = b2Vec2(aux::random_int(lower_left_corner.x, upper_right_corner.x) + aux::random_float(-1, 1, 4),
			aux::random_int(lower_left_corner.y, upper_right_corner.y) + aux::random_float(-1, 1, 4));
		//respawn_point = b2Vec2(0, 0);
		for (auto wall : walls) {
			acceptable = !aux::is_in_polygon(respawn_point, wall->get_vertices(), wall->get_orientation());
			acceptable &= (aux::dist_from_polygon(respawn_point, wall->get_vertices()) > 1);      // TODO
			if (!acceptable)
				break;
		}
	}
	return respawn_point;
}

Game::Game() : GameReadable() {
	// Contact filter
	//physics.SetContactFilter(&collision_filter);
	physics->SetContactListener(&contact_table);
	contact_table.set_collision_filter(&collision_filter);
	wall_player = create_player(-1, environment_team_id, sf::Color::White, "WALL");
	calc_intersection_ = [&](b2Vec2 _1, float _2) { return get_beam_intersection(_1, _2);};
}

GameReadable& Game::get_readable() {
	return static_cast<GameReadable&>(*this);
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
	b2Body* body = physics->CreateBody(&bodyDef);
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

Player* Game::create_player(int id, int team_id, sf::Color color, std::string name) {
	auto player = new Player();
	player->set_color(color);
	player->set_name(name);
	player->set_command_module(create_command_module());
	player->set_time_to_respawn(create_counter(0, -1));
	// Id
	player->set_id(id);
	player->set_team_id(team_id);
	players[player->get_id()] = player;
	return player;
}

CommandModule* Game::create_command_module() {
	auto command_module = new CommandModule();
	command_modules.insert(command_module);
	id_manager.set_id(command_module);
	return command_module;
}

Engine* Game::create_engine(b2Body* body, CommandModule* command_module, Counter* stamina, Effects* effs, float stamina_delay_) {
	auto engine = new Engine(body, command_module, stamina, effs, stamina_delay_);
	engines.insert(engine);
	id_manager.set_id(engine);
	return engine;
}

Counter* Game::create_counter(float val, float change_vel) {
	auto counter = new Counter();
	counter->set(val);
	counter->set_change_vel(change_vel);
	counters.insert(counter);
	//id_manager.set_id(counter);
	return counter;
}

DamageReceiver* Game::create_damage_receiver(b2Body* body, Counter* hp, Player* player, Effects* effs) {
	auto damage_receiver = new DamageReceiver(body, hp);
	damage_receiver->set_imm_frames(params["imm_frames"]);
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
	ship->set_id(player->get_id());
	ships_by_player_id[player->get_id()] = ship;

	// Create effects
	Effects_Prototype effects_prototype(effect_params);
	auto effs = create_effects(&effects_prototype);
	ship->set_effects(effs);
	

	// Player
	ship->set_player(player);

	// Hull def
	HullPrototype hull_prototype;
	if (hulls.count(player->get_hull_name()))
		hull_prototype = hulls[player->get_hull_name()];

	// Body
	auto body = create_round_body(pos, angle, hull_prototype.radius, hull_prototype.mass);
	collision_filter.add_body(body, CollisionFilter::STANDART, ship->get_player()->get_id());
	ship->set_body(body);

	// Command module
	auto command_module = player->get_command_module();

	

	// Hp
	auto hp = create_counter(hull_prototype.hp);
	hp->set_max(hull_prototype.hp);
	ship->set_hp(hp);

	// Stamina
	auto stamina = create_counter(hull_prototype.stamina, hull_prototype.stamina_recovery);
	stamina->set_max(hull_prototype.stamina);
	stamina->set_delay(0); 
	ship->set_stamina(stamina);

	// Energy
	auto energy = create_counter(hull_prototype.start_energy, hull_prototype.energy_regen);
	energy->set_max(hull_prototype.energy);
	ship->set_energy(energy);

	// Engine
	auto engine = create_engine(body, command_module, stamina, effs, hull_prototype.stamina_delay);
	engine->set_force_angular(hull_prototype.force_angular);
	engine->set_force_linear(hull_prototype.force_linear);
	ship->set_engine(engine);

	// Damage receiver
	auto damage_receiver = create_damage_receiver(body, hp, player, effs);
	ship->set_damage_receiver(damage_receiver);


	// Bonus slot
	auto bonus_slot = create_bonus_slot();
	ship->set_bonus_slot(bonus_slot);
	bonus_slot->set_effects(effs);
	bonus_slot->set(body, player);
	bonus_slot->set_stamina(stamina);
	bonus_slot->set_energy(energy);

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
	gun->set_effects(effs);
	gun->set_stamina(stamina);
	gun->set_energy(energy);
	ship->set_gun(gun);

	// Modules
	auto left = create_module(module_manager.get_prototype(Module::get_type_by_name(player->get_left_module_name())));
	auto right = create_module(module_manager.get_prototype(Module::get_type_by_name(player->get_right_module_name())));
	left->set(body, player);
	right->set(body, player);
	left->set_effects(effs);
	right->set_effects(effs);
	left->set_stamina(stamina);
	left->set_energy(energy);
	right->set_stamina(stamina);
	right->set_energy(energy);
	left->set_bind(CommandModule::LEFT_MODULE);
	right->set_bind(CommandModule::RIGHT_MODULE);
	ship->set_left_module(left);
	ship->set_right_module(right);

	return ship;
}

Wall* Game::create_wall(std::vector<b2Vec2> vertices, int orientation, float restitution, int type) {
	Wall* wall = new Wall();
	wall->set(physics, vertices, orientation, type);
	wall->get_body()->GetFixtureList()->SetRestitution(restitution);
	if (type == Wall::GHOST) {
		collision_filter.add_body(wall->get_body(), CollisionFilter::GHOST);
	}
	else {
		collision_filter.add_body(wall->get_body(), CollisionFilter::WALL);
	}
	walls.insert(wall);
	id_manager.set_id(wall);
	return wall;
}

Projectile* Game::create_projectile(ProjectileDef projectile_def) {
	// Creating body
	auto body = create_round_body(projectile_def.pos, projectile_def.angle, projectile_def.radius, projectile_def.mass);
	body->SetLinearVelocity(projectile_def.vel);
	collision_filter.add_body(body, CollisionFilter::PROJECTILE, projectile_def.player->get_id());

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

Event* Game::create_event(EventDef event_def, float playing_offset) {
	auto event = new Event();
	event->set_body(event_def.body);
	event->set_type(event_def.type);
	event->set_pos(event_def.pos);
	event->set_playing_offset(create_counter(playing_offset, 1));
	id_manager.set_id(event);
	events.insert(event);
	return event;
}

Effects* Game::create_effects(Effects_Prototype* val) {
	auto _effects = new Effects(val);
	effects.insert(_effects);
	return _effects;
}

Bonus* Game::create_bonus(Bonus_Def val) {
	auto ans = new Bonus;
	auto body = create_round_body(val.pos, 0, bonus_manager.get_prototype(val.type)->radius, 1);
	collision_filter.add_body(body, CollisionFilter::GHOST, 0);
	ans->set_id(val.get_id());
	ans->set_body(body);
	ans->set_type(val.type);
	bonuses.insert(ans);
	return ans;
}

BonusSlot* Game::create_bonus_slot() {
	auto bonus_slot = new BonusSlot;
	active_modules.insert(bonus_slot);

	auto counter = create_counter();
	counter->set_change_vel(-1);
	bonus_slot->set_recharge_counter(counter);

	bonus_slot->set_bonus_manager(&bonus_manager);
	bonus_slot->set_event_manager(&event_manager);

	id_manager.set_id(bonus_slot);

	return bonus_slot;
}

Module* Game::create_module(Module_Prototype* base) {
	auto module = module_manager.new_module(base);
	active_modules.insert(module);
	auto counter = create_counter();
	counter->set_change_vel(-1);
	module->set_recharge_counter(counter);
	module->set_projectile_manager(&projectile_manager);
	module->set_rocket_manager(&rocket_manager);
	module->set_event_manager(&event_manager);
	module->set_game_objects(static_cast<GameReadable*>(this));
	id_manager.set_id(module);
	return module;
}

Rocket* Game::create_rocket(Rocket_Def def) {
	auto rocket = new Rocket;
	id_manager.set_id(rocket);
	rockets.insert(rocket);
	// Params
	rocket->set_blast_force(def.base.blast_force);
	rocket->set_blast_radius(def.base.blast_radius);
	rocket->set_damage(def.base.damage);
	// Body
	rocket->set_player(def.player);
	auto body = create_round_body(def.pos + b2Vec2{0,0}, def.angle, def.base.radius, def.base.mass);
	body->SetLinearVelocity(def.vel);
	body->SetAngularVelocity(def.angle_vel);
	collision_filter.add_body(body, CollisionFilter::PROJECTILE, def.player->get_id());
	rocket->set_body(body);

	// Hp
	auto hp = create_counter(def.base.hp);
	hp->set_max(def.base.hp);
	rocket->set_hp(hp);

	// Stamina
	auto stamina = create_counter(def.base.stamina, def.base.stamina_recovery);
	stamina->set_max(def.base.stamina);
	stamina->set_delay(0);
	rocket->set_stamina(stamina);

	// Rocket brain
	auto command_module = create_command_module();
	auto brain = new RocketBrain(
		*command_module, 
		static_cast<const GameReadable&>(*this), 
		rocket, 
		def.base.range, 
		def.base.bin_search_accuracy);
	rocket_brains.insert(brain);
	rocket->set_rocket_brain(brain);


	// Engine
	auto engine = create_engine(body, command_module, stamina);
	engine->set_force_angular(0);
	engine->set_force_linear(def.base.force_linear);
	rocket->set_engine(engine);

	// Damage receiver
	auto damage_receiver = create_damage_receiver(body, hp, def.player);
	rocket->set_damage_receiver(damage_receiver);

	return rocket;
}

Forcefield* Game::create_forcefield(std::vector<b2Vec2> vertices, b2Vec2 force) {
	auto forcefield = new Forcefield();
	forcefield->set(physics, vertices, force);
	forcefield->get_body()->GetFixtureList()->SetRestitution(1);
	collision_filter.add_body(forcefield->get_body(), CollisionFilter::GHOST);
	forcefields.insert(forcefield);
	id_manager.set_id(forcefield);
	return forcefield;
}

void Game::delete_body(b2Body* body) {
	physics->DestroyBody(body);
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
void Game::delete_player(Player* player) {
	delete_command_module(player->get_command_module());
	delete_counter(player->get_time_to_respawn());
	players.erase(player->get_id());
	delete player;
}

void Game::delete_active_module(ActiveModule* active_module) {
	active_modules.erase(active_module);
	delete_counter(active_module->get_recharge_counter());
	delete active_module;
}

void Game::delete_command_module(CommandModule* _) {
	command_modules.erase(_);
	delete _;
}


void Game::delete_ship(Ship* ship) {
	ships_by_player_id.erase(ship->get_player()->get_id());
	delete_body(ship->get_body());
	delete_engine(ship->get_engine());
	delete_active_module(ship->get_gun());
	delete_damage_receiver(ship->get_damage_receiver());
	delete_counter(ship->get_hp());
	delete_counter(ship->get_stamina());
	delete_counter(ship->get_energy());
	delete_effects(ship->get_effects());
	delete_active_module(ship->get_bonus_slot());
	delete_active_module(ship->get_left_module());
	delete_active_module(ship->get_right_module());
	ships.erase(ship);
	delete ship;
}

void Game::delete_damage_receiver(DamageReceiver* damage_receiver) {
	damage_receivers.erase(damage_receiver);
	delete damage_receiver;
}

void Game::delete_counter(Counter* counter) {
	counters.erase(counter);
	delete counter;
}

void Game::delete_event(Event* event) {
	delete_counter(event->get_playing_offset());
	events.erase(event);
	delete event;
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

void Game::delete_rocket(Rocket* rocket) {
	rockets.erase(rocket);

	auto brain = (rocket->get_rocket_brain());
	delete_command_module(brain->get_command_module());
	rocket_brains.erase(brain);
	delete brain;

	delete_body(rocket->get_body());
	delete_engine(rocket->get_engine());
	delete_damage_receiver(rocket->get_damage_receiver());
	delete_counter(rocket->get_hp());
	delete_counter(rocket->get_stamina());
	delete rocket;
}

void Game::delete_wall(Wall* wall) {
	delete_body(wall->get_body());
	walls.erase(wall);
	delete wall;
}
void Game::delete_forcefield(Forcefield* field) {
	delete_body(field->get_body());
	forcefields.erase(field);
	delete field;
}




void Game::process_players() {
	// Creating ships
	for (auto player_pair : players) {
		auto player = player_pair.second;
		//if (player->get_name() == "DEBUG") {
		//	DEBUG_PRINT(player->get_is_alive());
		//	DEBUG_PRINT(player->get_time_to_respawn()->get());
		//	DEBUG_PRINT(player->get_command_module()->get_command(CommandModule::RESPAWN));
		//}
		if (!player->get_is_alive() && player->get_time_to_respawn()->get() < 0 
			&& player->get_command_module()->get_command(CommandModule::RESPAWN) 
			/*&& player->get_id() != -1*/) { // The player is not the wall


			player->set_is_alive(1);
			// creating ship
			auto ship = create_ship(player, get_rand_respawn_pos(), aux::random_float(0, 2 * b2_pi, 3));
		}
	}
}

void Game::process_ships() {
	// Deleting
	std::set<Ship*> ships_to_delete;
	for (auto ship : ships) {

		// Apply INSTANT_HP
		auto hp_eff = ship->get_effects()->get_effect(Effects::Types::INSTANT_HP)->get_counter();
		if (hp_eff->get() > 0) {
			ship->get_hp()->modify(hp_eff->get());
			hp_eff->set(0);

		}
		// Apply INSTANT_ENERGY
		auto st_eff = ship->get_effects()->get_effect(Effects::Types::INSTANT_ENERGY)->get_counter();
		if (st_eff->get() > 0) {
			ship->get_energy()->modify(st_eff->get());
			st_eff->set(0);
		}

		// Apply LASER
		if (ship->get_effects()->get_effect(Effects::Types::LASER)->get_counter()->get() > 0) {
			for (auto damage_receiver : damage_receivers) {
				if ( !(is_hostile_to(ship, damage_receiver)))
					continue;
				float angle = ship->get_body()->GetAngle();
				b2Vec2 pos = ship->get_body()->GetPosition();
				b2Vec2 target_pos = damage_receiver->get_body()->GetPosition();
				b2Vec2 intersection = get_beam_intersection(pos, angle);
				if (aux::dist_from_segment(target_pos, pos, intersection) < 
					ship->get_body()->GetFixtureList()->GetShape()->m_radius + 
					ship->get_effects()->get_effect(Effects::LASER)->get_param("width")) {
					if (damage_receiver->get_effects()) {
					    damage_receiver->get_effects()->update(Effects::WALL_BURN, ship->get_effects()->get_effect(Effects::WALL_BURN)->get_param("duration"));
					}
					damage_receiver->damage(ship->get_effects()->get_effect(Effects::LASER)->get_param("damage"), ship->get_player());
				}
			}
		}
		
		// Spiked walls
		for (auto wall : walls) {
			if (contact_table.check(ship->get_body(), wall->get_body())) {
				if (wall->get_type() == Wall::SPIKED && 
					ship->get_effects()->get_effect(Effects::WALL_BURN)->get_counter()->get() < b2_epsilon 
					&& wall_player->is_hostile_to(*ship->get_player())) {
					ship->get_damage_receiver()->damage(params["wall_damage"], ship->get_damage_receiver()->get_last_hit());
					ship->get_effects()->update(Effects::WALL_BURN, ship->get_effects()->get_effect(Effects::WALL_BURN)->get_param("duration"));
					if (ship->get_hp()->get() < b2_epsilon) {
						wall_player->add_kill();
					}
				}
			}
		}

		// Bonus activating
		if (ship->get_player()->get_command_module()->get_command(CommandModule::BONUS_ACTIVATION)) {
			if (ship->get_bonus_slot()) {
				if (ship->get_bonus_slot()->get_current_bonus() == Bonus::LASER)
					event_manager.create_event(EventDef(Event::LASER, ship->get_body()));
				ship->get_bonus_slot()->activate();				
			}
		}

		// Apply CHARGE
		if (ship->get_effects()->get_effect(Effects::Types::CHARGE)->get_counter()->get() > 0) { // Apply CHARGE
			collision_filter.change_body(ship->get_body(), CollisionFilter::PROJECTILE);
			if (ship->get_effects()->get_effect(Effects::Types::CHARGE)->get_counter()->get() < 0.05) { // TODO
				collision_filter.change_body(ship->get_body(), CollisionFilter::STANDART);
				ship->get_effects()->get_effect(Effects::Types::CHARGE)->get_counter()->set(0);
			}
			for (auto damage_receiver : damage_receivers) {
				if (contact_table.check(ship->get_body(), damage_receiver->get_body()) &&
					ship->get_player()->is_hostile_to(*damage_receiver->get_player())) {
					ship->get_effects()->update(Effects::WALL_BURN, ship->get_effects()->get_effect(Effects::WALL_BURN)->get_param("duration"));
					damage_receiver->damage(ship->get_effects()->get_effect(Effects::CHARGE)->get_param("damage"), ship->get_player());
				}
			}
		}
		// Death, Checking for < zero hp
		if (ship->get_hp()->get() <= 0) {
			ships_to_delete.insert(ship);
			auto player = ship->get_player();
			player->add_death();
			if (ship->get_damage_receiver()->get_last_hit() != nullptr && ship->get_damage_receiver()->get_last_hit()->is_hostile_to(*ship->get_player())) {
				ship->get_damage_receiver()->get_last_hit()->add_kill();

				if (ship->get_damage_receiver()->get_last_hit() && ship->get_damage_receiver()->get_last_hit()->get_is_alive()) {
					ships_by_player_id[ship->get_damage_receiver()->get_last_hit()->get_id()]->get_energy()->modify(params["energy_on_kill"]);
				}
			}
			player->set_is_alive(0);

			player->get_time_to_respawn()->set(3);			// TODO: add respawn time to config

			event_manager.create_event(EventDef(Event::DEATH, nullptr, ship->get_body()->GetPosition()));
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
		bool do_break = false;
		for (auto damage_receiver : damage_receivers) {
			if (contact_table.check(projectile->get_body(), damage_receiver->get_body()) &&
				is_hostile_to(projectile, damage_receiver)) {
				damage_receiver->damage(projectile->get_damage(), projectile->get_player());
				damage_receiver->apply_effects(projectile->get_effects_def());
			}
		}
		// Checking for wall collision
		for (auto wall : walls) {
			if (contact_table.check(projectile->get_body(), wall->get_body())) {
				projectiles_to_delete.insert(projectile);
				do_break = true;
				break;
			}

		}
		if (do_break) {
			continue;
		}
		// Checking for ship collision
		for (auto ship : ships) {
			if (projectile->get_player()->get_id() != ship->get_player()->get_id() &&
				contact_table.check(projectile->get_body(), ship->get_body())) {
				projectiles_to_delete.insert(projectile);
				do_break = true;
				break;
			}

		}
		if (do_break) {
			continue;
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

void Game::process_projectile_manager() {
	ProjectileDef projectile_def;
	while (projectile_manager.get_next(projectile_def)) {
		create_projectile(projectile_def);
	}
}

void Game::process_event_manager() {
	EventDef event_def;
	while (event_manager.get_next(event_def)) {
		create_event(event_def);
	}
}

void Game::process_physics() {
	auto contact_table_prev = contact_table;
	contact_table.reset();
	for (b2Contact* contact = physics->GetContactList(); contact; contact = contact->GetNext()) {
		contact->SetRestitution(contact->GetFixtureA()->GetRestitution() *
			contact->GetFixtureB()->GetRestitution());
	}
	physics->Step(dt, 10, 10);
	std::set<b2Body*> hit_objects; // To avoid repetitions
	for (b2Contact* contact = physics->GetContactList(); contact; contact = contact->GetNext()) {
		// Hit event
		if (
			// Check for contact
			contact_table.check(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody()) &&
			!contact_table_prev.check(contact->GetFixtureA()->GetBody(), contact->GetFixtureB()->GetBody()) &&
			collision_filter.ShouldCollide(contact->GetFixtureA(), contact->GetFixtureB())
			) {
			int numPoints = contact->GetManifold()->pointCount;
			b2WorldManifold worldManifold;
			contact->GetWorldManifold(&worldManifold);
			b2Vec2 pos = worldManifold.points[0];
			if (b2Distance(b2Vec2_zero, pos) > 1e5 || abs(pos.x) < b2_epsilon || abs(pos.y) < b2_epsilon) 
				continue; // TODO: all collision points with zero x- or y-coordinates are now banned
			// Adding used objects
			hit_objects.insert(contact->GetFixtureA()->GetBody());
			hit_objects.insert(contact->GetFixtureB()->GetBody());
			// Creating event
			event_manager.create_event(EventDef(Event::WALL_HIT, nullptr, pos));
		}
	}
}

void Game::process_counters() {
	for (auto counter : counters)
		counter->step(dt);
}

void Game::process_events() {
	std::set<Event*> events_to_delete;
	for (auto event : events) {
		if (!event->is_alive())
			events_to_delete.insert(event);
	}
	for (auto event : events_to_delete)
		delete_event(event);
}

void Game::process_effects() {
	for (auto eff : effects) {
		eff->step(dt);
	}
}

void Game::process_rockets() {
	std::vector<Rocket*> rockets_to_delete;
	for (auto rocket : rockets) {
		bool do_break = false;
		for (auto wall : walls) {
			if (contact_table.check(rocket->get_body(), wall->get_body())) {
				rockets_to_delete.push_back(rocket);
				do_break = true;
				break;
			}
		} 
		if (do_break) {
			continue;
		}
		// Checking for ship collision
		for (auto ship : ships) {
			if (rocket->get_player()->get_id() != ship->get_player()->get_id() &&
				contact_table.check(rocket->get_body(), ship->get_body())) {
				rockets_to_delete.push_back(rocket);
				do_break = true;
			}
		}
		if (do_break) {
			continue;
		}
		// Checking hp
		if (rocket->get_hp()->get() <= 0) {
			rockets_to_delete.push_back(rocket);
		}
	}
	for (auto rocket : rockets_to_delete) {
		for (auto receiver : damage_receivers) {
			if ((rocket->get_body()->GetWorldPoint({ 0,0 }) - receiver->get_body()->GetWorldPoint({ 0,0 })).Length() < rocket->get_blast_radius()) {
				receiver->damage(rocket->get_damage(), rocket->get_player());
				/*if (rocket->get_player() != receiver->get_player()) {
					receiver->damage(rocket->get_damage(), rocket->get_player());
				} else {
					receiver->damage(rocket->get_damage(), receiver->get_last_hit());
				}*/
				b2Vec2 unit = (receiver->get_body()->GetWorldPoint({ 0,0 }) - rocket->get_body()->GetWorldPoint({ 0,0 }));
				unit.Normalize();
				b2Vec2 impulse = rocket->get_blast_force() * (std::min(receiver->get_body()->GetMass(), 1.f)) * unit;
				receiver->get_body()->ApplyLinearImpulseToCenter(impulse, 1);
			}
		}
	}
	for (auto rocket : rockets_to_delete) {
		delete_rocket(rocket);
	}
}

void Game::process_rocket_brains() {
	for (auto brain : rocket_brains) {
		brain->compute_action();
	}
}

void Game::process_rocket_manager() {
	Rocket_Def def;
	while (rocket_manager.get_next(def)) {
		create_rocket(def);
	}
}

void Game::process_forcefields() {
	// TODO: make efficient
	for (auto field : forcefields) {
		for (auto body = physics->GetBodyList(); body; body = body->GetNext()) {
			field->apply(body, dt);
		}
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
	// Pick up
	for (auto bonus : bonuses) {
		for (auto ship : ships) {
			if (contact_table.check(bonus->get_body(), ship->get_body())) {
				ship->get_bonus_slot()->add_bonus(bonus->get_type());
				bonuses_to_delete.push_back(bonus);
				event_manager.create_event(EventDef(Event::BONUS_PICKUP, nullptr, bonus->get_body()->GetPosition()));
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
	if (game_mode.is_game_finished()) {
		return;
	}
	game_mode.step();
	time += dt;
	process_physics();
	process_players();
	process_ships();
	process_engines();
	process_projectiles();
	process_projectile_manager();
	process_event_manager();
	process_counters();
	process_events();
	process_effects();
	process_bonuses();
	process_bonus_manager();
	process_active_modules();
	process_rocket_brains();
	process_rockets();
	process_rocket_manager();
	process_forcefields();
}

float Game::get_dt() {
	return dt;
}

void Game::set_dt(float _dt) {
	dt = _dt;
}

void Game::set_time(std::map<int, int>* _time) {
	connection_time = _time;
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
	for (auto module : active_modules) {
		delete module;
	}
	active_modules = {};
	// Clear counters
	for (auto counter : counters)
		delete counter;
	counters = {};
	// Clear events
	for (auto event : events)
		delete event;
	events = {};
	// Clear bonuses
	for (auto bonus : bonuses)
		delete bonus;
	bonuses = {};
	for (auto effect : effects) {
		delete effect;
	}
	effects = {};
	for (auto rocket : rockets) {
		delete rocket;
	}
	rockets = {};
	for (auto rocket_brain : rocket_brains) {
		delete rocket_brain;
	}
	rocket_brains = {};

	// Clear physics
	delete physics;
	physics = new b2World(b2Vec2_zero);

	contact_table = ContactTable();
	physics->SetContactListener(&contact_table);
	collision_filter = CollisionFilter();
	contact_table.set_collision_filter(&collision_filter);
}

void Game::wipe_map() {
	clear();
	delete team_manager;
	for (auto wall : walls) {
		delete wall;
	}
	walls = {};
	for (auto field : forcefields) {
		delete field;
	}
	forcefields = {};
}

bool Game::load_map(std::string path) {
	walls.clear();
	map_path = path;
	std::ifstream file_input(path);
	std::stringstream input = aux::comment(file_input);
	int wall_id = 0;
	int forcefield_id = 0;
	bool corner_init = 0;

	// Parsing
	std::string symbol;
	while (input >> symbol) {
		if (symbol == "END")
			break;
		// Wall
		if (symbol == "FORCEFIELD") {
			std::string symbol_1;
			std::vector<b2Vec2> points;
			b2Vec2 force;
			while (input >> symbol_1) {
				if (symbol_1 == "END") {
					break;
				}
				if (symbol_1 == "FORCE") {
					input >> force.x >> force.y;
					continue;
				}
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
				std::cerr << "Game::load_map: unknown symbol " << symbol_1 << "\n";
				return false;
			}
			auto ff = create_forcefield(points, force);
			ff->set_id(forcefield_id);

			float i_dunno = 0.3;
			auto pts = ff->get_pts_for_grid(i_dunno);
			DEBUG_PRINT("in grid: " << pts.size() << std::endl);
			for (auto pt : pts) {
				ffield_spawnpoint_grid.push_back(pt);
			}

			forcefield_id++;
			continue;
		}
		if (symbol == "WALL") {
			std::string symbol_1;
			std::vector<b2Vec2> points;
			int orientation = Wall::OUTER;
			int type = Wall::STANDART;
			float restitution = params["wall_restitution"];
;			while (input >> symbol_1) {
				if (symbol_1 == "END")
					break;
				if (symbol_1 == "SPIKED") {
					type = Wall::SPIKED;
					continue;
				}
				if (symbol_1 == "STANDART") {
					type = Wall::STANDART;
					continue;
				}
				if (symbol_1 == "GHOST") {
					type = Wall::GHOST;
					continue;
				}
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
			create_wall(points, orientation, restitution, type)->set_id(wall_id);

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

	unsigned char gun_alias = 1, hull_alias = 1;
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
					//std::cout << "Game::load_parameters: failed to read " + symbol_name + "\n";

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
					//std::cout << "Game::load_parameters: failed to read " + symbol_name + "\n";

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
		if (symbol == "INVIS_FLAG") {
			Effects::invis_flag = Effects::Invisibility_Flag::SHOW_NO;
			while (input >> symbol) {
				if (symbol == "END")
					break;
				if (symbol == "SHOW_EFFECTS") {
					Effects::invis_flag = Effects::invis_flag || Effects::Invisibility_Flag::SHOW_EFFECTS;
					std::cout << "Invisibility flag SHOW_EFFECTS set\n";
					continue;
				}
				if (symbol == "SHOW_BOOST") {
					Effects::invis_flag = Effects::invis_flag || Effects::Invisibility_Flag::SHOW_BOOST;
					std::cout << "Invisibility flag SHOW_BOOST set\n";
					continue;
				}
				std::cerr << "Error: Unknown invisibility flag " << symbol << "\n";
			}
			continue;
		}
		if (symbol == "PARAMETERS") {
			while (input >> symbol) {
				if (symbol == "END")
					break;
				float temp;
				input >> temp;
				std::cout << "Parameter " << symbol << " set to " << temp << " in PARAMETERS \n";
				params[symbol] = temp;
			}
			continue;
		}
		if (symbol == "GAME_MODE") {
			int max_players = 12;

			while (input >> symbol) {
				DEBUG_PRINT("GAME_MODE " << symbol)
				if (symbol == "END") {
					break;
				}
				if (symbol == "MAX_PLAYERS") {
					input >> max_players;
					continue;
				}
				if (symbol == "FFA") {
					delete team_manager;
					team_manager = new FFATeamManager(max_players);
					continue;
				}
			}
			continue;
		}
		if (symbol == "EFFECT_TYPES") {
			while (input >> symbol) {
				if (symbol == "END")
					break;
				int type_ = Effects::get_effect_type(symbol);
				effect_params.effects[type_].set_type(Effects::Algebraic_Type::ADDITIVE);
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
						else if (temp == "NO_OVERLAY") {
							effect_params.effects[type_].set_type(Effects::Algebraic_Type::NO_OVERLAY);
						}
					}
					else {
						float temp;
						input >> temp;
						std::cout << "Parameter " << symbol << " set to " << temp  << " in effect number " << type_ << "\n";
						effect_params.effects[type_].set_param(symbol, temp);
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
			}
			bonus_manager.add_prototype(bonus_prototype);
			continue;
		}
		// Modules
		if (symbol == "MODULE") {
			std::string name;
			Module_Prototype prototype;
			if (!(input >> name)) {
				std::cerr << "Game::load_parameters: failed to read MODULE name\n";
				return false;
			}
			prototype.type = Module::get_type_by_name(name);
			while (input >> symbol) {
				if (symbol == "END")
					break;
				if (symbol == "EFFECTS") {
					std::cout << "Module effect prototypes are unused for now\n";
					prototype.effects_prototype = read_effect_prototype();
				}
				else if (symbol == "RECHARGE_TIME") {
					float val;
					if (!(input >> val)) {
						std::cerr << "Game::load_parameters: failed to read RECHARGE_TIME\n";
					}
					prototype.recharge_time = val;
				}
				else if (symbol == "STAMINA_COST") {
					float val;
					if (!(input >> val)) {
						std::cerr << "Game::load_parameters: failed to read STAMINA_COST\n";
					}
					prototype.stamina_cost = val;
				}
				else if (symbol == "ENERGY_COST") {
					float val;
					if (!(input >> val)) {
						std::cerr << "Game::load_parameters: failed to read ENERGY_COST\n";
					}
					prototype.energy_cost = val;
				}
				else if (symbol == "STAMINA_DELAY") {
					float val;
					if (!(input >> val)) {
						std::cerr << "Game::load_parameters: failed to read STAMINA_DELAY\n";
					}
					prototype.stamina_delay = val;
				}
				else {
					float temp;
					input >> temp;
					std::cout << "Parameter " << symbol << " set to " << temp << " in module prototype " << name << "\n";
					prototype.params[symbol] = temp;
				}
			}
			module_manager.add_prototype(prototype);
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
			guns[name].alias = gun_alias;
			gun_by_alias[gun_alias] = name;
			gun_alias++;
			while (input >> symbol) {
				if (symbol == "END")
					break;
				if (symbol == "EFFECTS") {
					guns[name].effect_prototype = read_effect_prototype();
				}
				read_symbol("RECHARGE", guns[name].recharge_time);
				read_symbol("DAMAGE", guns[name].damage);
				read_symbol("STAMINA_CONSUMPTION", guns[name].stamina_cost);
				read_symbol("STAMINA_DELAY", guns[name].stamina_delay);
				read_symbol("PROJECTILE_MASS", guns[name].projectile_mass);
				read_symbol("PROJECTILE_VEL", guns[name].projectile_vel);
				read_symbol("PROJECTILE_RADIUS", guns[name].projectile_radius);
				read_symbol("PROJECTILE_HP", guns[name].projectile_hp);
				read_symbol("ENERGY_CONSUMPTION", guns[name].energy_cost);
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
			hulls[name].alias = hull_alias;
			hull_by_alias[hull_alias] = name;
			hull_alias++;
			while (input >> symbol) {
				if (symbol == "END")
					break;
				read_symbol("HP", hulls[name].hp);
				read_symbol("MASS", hulls[name].mass);
				read_symbol("RADIUS", hulls[name].radius);
				read_symbol("STAMINA", hulls[name].stamina);
				read_symbol("STAMINA_RECOVERY", hulls[name].stamina_recovery);
				read_symbol("LINEAR_FORCE", hulls[name].force_linear);
				read_symbol("ANGULAR_FORCE", hulls[name].force_angular);
				read_symbol("STAMINA_DELAY", hulls[name].stamina_delay);
				read_symbol("ENERGY", hulls[name].energy);
				read_symbol("START_ENERGY", hulls[name].start_energy);
				read_symbol("ENERGY_REGEN", hulls[name].energy_regen);
			}
			continue;
		}
		std::cerr << "Game::load_parameters: unknown symbol " << symbol << "\n";
		return false;
	}
	is_friendly_fire = params["friendly_fire "] > 0.5f; // TODO : fix parser
	return true;
}



std::string Game::encode() {
	std::string message = "";

	// Map path
	message += "M" + map_path + " ";

	// Game finished
	if (game_mode.is_game_finished()) {
		message += "F ";
	}

	// Time left (T)
	message += "T" + aux::write_int(game_mode.get_time_left());

	// Global vars (G)
	message += "G" + aux::write_int8(char(is_friendly_fire));

	// Players (P)
	for (auto player : players) {
		message += "P";
		// Id
		message += aux::write_int(player.first);
		// Team Id
		message += aux::write_int(player.second->get_team_id());
		// Color
		sf::Color color = player.second->get_color();
		color.r = std::max(0, color.r - 1);
		color.g = std::max(0, color.g - 1);
		color.b = std::max(0, color.b - 1);
		message += aux::write_int8(color.r);
		message += aux::write_int8(color.g);
		message += aux::write_int8(color.b);
		// Name
		message += player.second->get_name() + " ";
		// Hull
		message += hulls[player.second->get_hull_name()].alias;
		// Gun
		message += guns[player.second->get_gun_name()].alias;
		// Deaths & kills
		message += aux::write_short(player.second->get_deaths());
		message += aux::write_short(player.second->get_kills());
		// Time to respawn
		message += aux::write_int8(int(player.second->get_time_to_respawn()->get() + 0.99));
		// Is alive
		message += aux::write_int8(int(player.second->get_is_alive()));
		// Last connection time
		message += aux::write_int(connection_time->operator[](player.first));
	}

	// Ships (S)
	for (auto ship : ships) {
		message += "S";
		// Id
		message += aux::write_int(ship->get_id());
		// Player id
		message += aux::write_int(ship->get_player()->get_id());
		// Pos
		message += aux::write_float(ship->get_body()->GetPosition().x, 2);
		message += aux::write_float(ship->get_body()->GetPosition().y, 2);
		// Linear velocity
		message += aux::write_float(ship->get_body()->GetLinearVelocity().x, 2);
		message += aux::write_float(ship->get_body()->GetLinearVelocity().y, 2);
		// Angle
		message += aux::write_float(aux::vec_to_angle(aux::angle_to_vec(ship->get_body()->GetAngle())), 3);
		// Radius
		message += aux::write_float(ship->get_body()->GetFixtureList()->GetShape()->m_radius, 2);
		// Commands
		message += aux::mask_to_string(ship->get_player()->get_command_module()->get_active()) + " ";
		// Effects
		message += aux::mask_to_string(ship->get_effects()->get_mask()) + " ";
		// Bonus slot
		message += aux::write_int8(ship->get_bonus_slot()->get_current_bonus());
		// Modules
		message += aux::write_int8(ship->get_left_module()->get_type());
		message += aux::write_float(ship->get_left_module()->get_recharge_counter()->get(), 2);
		message += aux::write_float(ship->get_left_module()->get_recharge_time(), 2);

		message += aux::write_int8(ship->get_right_module()->get_type());
		message += aux::write_float(ship->get_right_module()->get_recharge_counter()->get(), 2);
		message += aux::write_float(ship->get_right_module()->get_recharge_time(), 2);


		// Hp
		message += aux::write_short((int)ship->get_hp()->get());
		message += aux::write_short((int)ship->get_hp()->get_max());
		// Stamina
		message += aux::write_short((int)ship->get_stamina()->get());
		message += aux::write_short((int)ship->get_stamina()->get_max());
		// Energy
		message += aux::write_short((int)ship->get_energy()->get());
		message += aux::write_short((int)ship->get_energy()->get_max());
	}

	// Efficient projectiles
	std::map<int, std::string> player_packs;
	for (auto projectile : projectiles) {
		int player_id = projectile->get_player()->get_id();
		// Id
		player_packs[player_id] += aux::write_short(projectile->get_id() % 5000);
		// Pos
		player_packs[player_id] += aux::write_float(projectile->get_body()->GetPosition().x, 2);
		player_packs[player_id] += aux::write_float(projectile->get_body()->GetPosition().y, 2);
		// Angle
		player_packs[player_id] += aux::write_float(aux::vec_to_angle(aux::angle_to_vec(projectile->get_body()->GetAngle())), 3);
		// Radius
		player_packs[player_id] += aux::write_float(projectile->get_body()->GetFixtureList()->GetShape()->m_radius, 2);
	}
	for (auto pack : player_packs) {
		message += "p";
		message += aux::write_int(pack.first);
		message += aux::write_int8(pack.second.size() / 10);
		message += pack.second;	
	}
	// Rockets (r)
	for (auto rocket : rockets) {
		message += "r";
		// Id
		message += aux::write_short(rocket->get_id() % 15000);
		// Player id
		message += aux::write_int(rocket->get_player()->get_id());
		// Pos
		message += aux::write_float(rocket->get_body()->GetPosition().x, 2);
		message += aux::write_float(rocket->get_body()->GetPosition().y, 2);
		// Angle
		message += aux::write_float(aux::vec_to_angle(aux::angle_to_vec(rocket->get_body()->GetAngle())), 3);
		// Radius
		message += aux::write_float(rocket->get_body()->GetFixtureList()->GetShape()->m_radius, 2);
	}

	// Bonuses (b)
	for (auto bonus : bonuses) {
		message += "b";
		// Id
		message += aux::write_int8(bonus->get_id());
		// Position
		message += aux::write_float(bonus->get_body()->GetPosition().x, 2);
		message += aux::write_float(bonus->get_body()->GetPosition().y, 2);
		// Type
		message += aux::write_int8(bonus->get_type());
	}

	// Events (e)
	for (auto event : events) {
		message += "e";
		// Id
		message += aux::write_short(event->get_id());
		// Type
		message += aux::write_int8(event->get_type());
		// Pos
		message += aux::write_float(event->get_pos().x, 2);
		message += aux::write_float(event->get_pos().y, 2);
	}
	std::string ans = aux::write_short(message.size());
	ans += (message);
	return ans;
}

Player* Game::create_new_player(int id, int team_id, sf::Color color, std::string name, std::string gun_name, std::string hull_name,
	std::string left_module, std::string right_module) {
	Player* player = create_player(id, team_id, color, name);
	player->set_gun_name(gun_name);
	player->set_hull_name(hull_name);
	player->set_left_module_name(left_module);
	player->set_right_module_name(right_module);
	players[id] = player;
	player->set_is_alive(0);
	player->get_time_to_respawn()->set(3);
	return player;
}

Player* Game::player_by_id(int id) {
	if (!players.count(id))
		return nullptr;
	return players[id];
}

void Game::clear_player(int id) {
	id_list.erase(id);
	delete player_reprs[id];
	player_reprs.erase(id);

	if (players.count(id) == 0) {
		return;
	}

	// Deleting ships
	std::deque<Ship*> ships_to_delete;

	for (auto ship : ships)
		if (ship->get_player() == players[id])
			ships_to_delete.push_back(ship);

	for (auto ship : ships_to_delete)
		delete_ship(ship);
	auto res = *players.find(id);

	delete_player(res.second);
}

PlayerHandle Game::new_player__by_handle(PlayerDefHint def) {
	PlayerHandle res;
	res.game = this;

	if (id_list.count(def.id)) {
		std::cerr << "Game::new_player__by_handle error: id collision with player.name = " << def.name << "\n";
		return res;
	}
	id_list.insert(def.id);
	
	player_reprs[def.id] = new PlayerRepr(def);
	res.repr = player_reprs[def.id];
	return res;
}

PlayerHandle Game::player_by_id__by_handle(int id) {
	PlayerHandle res;
	res.game = this;
	res.repr = player_reprs[id];
	return res;
}

void PlayerHandle::delete_player() {
	game->clear_player(repr->player->get_id());
}
void PlayerHandle::apply_command(int command, int val) {
	if (repr->stage == WAITING_IN_LOBBY && command == CommandModule::RESPAWN && val) {
		PlayerDefHint def = repr->def_to_apply_next;

		// TODO: team manager

		auto maybe_team_params = game->team_manager->assign_team(def.id, def.team_name_hint);
		if (!maybe_team_params) {
			DEBUG_PRINT("player: " << def.name << " not created!");
			return;
		}
		auto team_params = maybe_team_params.value();

		Player* player = game->create_new_player(def.id, team_params.team_id, team_params.team_color, def.name, def.gun_name, def.hull_name, def.left_module_name, def.right_module_name);
		DEBUG_PRINT("player: " << def.name << " created");
		DEBUG_PRINT(player->get_id() << " "  << player->get_color().r <<" "<< player->get_color().r <<" "<< player->get_color().r);
		DEBUG_PRINT(def.hull_name);

		repr->stage = PLAYING;
		repr->player = player;
	}
	if (repr->stage == PLAYING) {
		repr->player->get_command_module()->set_command(command, val);
	}
}
void PlayerHandle::apply_def() {
	PlayerDefHint& def = repr->def_to_apply_next;

	if (repr->stage == PLAYING) {
		repr->player->set_name(def.name);
	}
	if (repr->stage == PLAYING && !repr->player->get_is_alive()) {
		Player* player = repr->player;
		player->set_gun_name(def.gun_name);
		player->set_hull_name(def.hull_name);
		player->set_left_module_name(def.left_module_name);
		player->set_right_module_name(def.right_module_name);
	}
}

bool Game::is_game_finished() {
	return game_mode.is_game_finished();
}

Game::~Game() {
	wipe_map();
}


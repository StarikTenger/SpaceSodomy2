#include "GameClient.h"
#include <iostream>
#include <thread>

std::string GameClient::get_gun_name() {
	return gun_name;
}

std::string GameClient::get_hull_name() {
	return hull_name;
}

int GameClient::get_aim_conf() {
	return aim_conf;
}

int GameClient::get_aim_opacity() {
	return aim_opacity;
}

std::string GameClient::get_left_module_name() {
	return left_module_name;
}
std::string GameClient::get_right_module_name() {
	return right_module_name;
}

void GameClient::set_network_information_active(bool _active) {
	network_information_active = _active;
}

std::string GameClient::get_bonus_texture_name(int val) {
	return bonus_textures[val];
}

void GameClient::set_gun_name(std::string val) {
	gun_name = val;
}

void GameClient::set_hull_name(std::string val) {
	hull_name = val;
}

void GameClient::set_left_module_name(std::string val) {
	left_module_name = val;
}

void GameClient::set_right_module_name(std::string val) {
	right_module_name = val;
}

void GameClient::set_aim_opacity(int _opacity) {
	aim_opacity = _opacity;
}

void GameClient::set_aim_conf(int _conf) {
	aim_conf = _conf;
}

bool GameClient::get_network_information_active() {
	return network_information_active;
}

void GameClient::display(int id) {
	GameDrawable::display(id);
}

void GameClient::update_state(std::string source) {
	// Hp value to compare, is used to catch damage event
	float hp_prev = 0;
	if (get_ship(my_id)) {
		hp_prev = get_ship(my_id)->get_hp()->get();
	}
	// Projectiles that are not destroyed will be erased
	struct Disappear_Animation {
		enum Type {
			PROJECTILE,
			ROCKET
		};
		int type;
		b2Vec2 pos;
		sf::Color color;
		float radius;
	};
	std::map<int, Disappear_Animation> destroyed_objects;
	auto manage_destroyed_object = [&](int id) {
		if (destroyed_objects.count(id))
			destroyed_objects.erase(destroyed_objects.find(id));
	};
	for (auto projectile : projectiles) {
		destroyed_objects[projectile->get_id()].type = Disappear_Animation::PROJECTILE;
		destroyed_objects[projectile->get_id()].pos = projectile->get_body()->GetPosition();
		destroyed_objects[projectile->get_id()].color = projectile->get_player()->get_color();
		destroyed_objects[projectile->get_id()].radius = projectile->get_body()->GetFixtureList()->GetShape()->m_radius;
	}
	for (auto rocket : rockets) {
		destroyed_objects[rocket->get_id()].type = Disappear_Animation::ROCKET;
		destroyed_objects[rocket->get_id()].pos = rocket->get_body()->GetPosition();
		destroyed_objects[rocket->get_id()].color = rocket->get_player()->get_color();
		destroyed_objects[rocket->get_id()].radius = rocket->get_body()->GetFixtureList()->GetShape()->m_radius;
	}

	// Actual events for new event managing
	std::set<int> active_events;
	for (auto e : events)
		active_events.insert(e->get_id());

	// First clear
	clear();

	// Creating stringstream
	std::stringstream stream;
	stream << source;

	// Parsing source
	std::string symbol;
	int msg_size = aux::read_short(stream);
	while (symbol = stream.get(), !stream.eof()) {
		// Map
		if (symbol == "M") {
			std::string path;
			stream >> path;
			if (map_path != path) {
				map_path = path;
				load_map(map_path);
				for (int i = 0; i < path.size(); i++) {
					if (path[i] == '/' || path[i] == '.') {
						path[i] = '_';
					}
				}
				auto load_func = [](GameDrawable* game_client) {
					game_client->load_wall_textures();
				};
				std::thread load_thread(load_func, this);
				load_thread.detach();
			}
		}
		// Player
		if (symbol == "P") {
			// Id
			//stream.get();
			int id;
			id = aux::read_int(stream);
			// Color
			sf::Color color;
			int r = aux::read_int8(stream), g = aux::read_int8(stream), b = aux::read_int8(stream);
			color = sf::Color(r, g, b);
			// Name
			std::string name;
			stream >> name;
			stream.get();
			// Hull
			std::string hull = hull_by_alias[stream.get()];
			// Gun
			std::string gun = gun_by_alias[stream.get()];
			// Deaths, kills & etc
			int deaths, kills, time_to_respawn, is_alive, connection_time;
			deaths = aux::read_short(stream);
			kills = aux::read_short(stream);
			time_to_respawn = aux::read_int8(stream);
			is_alive = aux::read_int8(stream);
			connection_time = aux::read_int(stream);

			// Creating player
			Player* player = create_player(id);
			player->set_color(color);
			player->set_name(name);
			player->set_hull_name(hull);
			player->set_gun_name(gun);
			player->set_deaths(deaths);
			player->set_kills(kills);
			player->set_is_alive(is_alive);
			player->get_time_to_respawn()->set(float(time_to_respawn));
			player->set_ping(aux::get_milli_count() - connection_time);
		}
		// Ship
		if (symbol == "S") {
			// Ids
			//stream.get();
			int id, player_id;
			id = aux::read_int(stream);
			player_id = aux::read_int(stream);
			// Pos
			b2Vec2 pos;
			pos.x = aux::read_float(stream, 2);
			pos.y = aux::read_float(stream, 2);
			// Linear velocity
			b2Vec2 linvel;
			linvel.x = aux::read_float(stream, 2);
			linvel.y = aux::read_float(stream, 2);
			// Angle
			float angle;
			angle = aux::read_float(stream, 3);
			// Radius
			float radius;
			radius = aux::read_float(stream, 2);
			// Commands
			std::string commands_stringed;
			stream >> commands_stringed;
			// Effects
			std::string effects_stringed;
			stream >> effects_stringed;
			stream.get();
			// Bonus slot
			int bonus;
			bonus = aux::read_int8(stream);
			// Modules
			int left_module, right_module;
			float left_module_time, left_module_max_time;
			float right_module_time, right_module_max_time;

			left_module = aux::read_int8(stream);
			left_module_time = aux::read_float(stream, 2);
			left_module_max_time = aux::read_float(stream, 2);
			right_module = aux::read_int8(stream);
			right_module_time = aux::read_float(stream, 2);
			right_module_max_time = aux::read_float(stream, 2);
			// Hp
			float hp;
			hp = aux::read_short(stream);
			float max_hp;
			max_hp = aux::read_short(stream);
			float stamina;
			stamina = aux::read_short(stream);
			float max_stamina;
			max_stamina = aux::read_short(stream);
			float energy;
			energy = aux::read_short(stream);
			float max_energy;
			max_energy = aux::read_short(stream);

			auto ship = create_ship(players[player_id], pos, angle);

			// Set bonus to HUD bonus slot
			if (player_id == my_id && ship->get_bonus_slot()->get_current_bonus() != bonus) {
				auto bonus_texture_name = get_bonus_texture_name(bonus);
				auto bonus_texture = draw->get_texture(bonus_texture_name);
				gui->get<tgui::Picture>("Bonus")->getRenderer()->setTexture(*bonus_texture);
			}

			ship->set_id(id);
			ship->get_body()->SetLinearVelocity(linvel);
			ship->get_body()->GetFixtureList()->GetShape()->m_radius = radius;
			ship->get_hp()->set(hp);
			ship->get_stamina()->set(stamina);
			ship->get_energy()->set(energy);
			ship->get_hp()->set_max(max_hp);
			ship->get_stamina()->set_max(max_stamina);
			ship->get_energy()->set_max(max_energy);
			ship->get_bonus_slot()->set_current_bonus(bonus);
			ship->get_left_module()->set_type(static_cast<Module::Type>(left_module));
			ship->get_left_module()->get_recharge_counter()->set(left_module_time);
			ship->get_left_module()->get_recharge_counter()->set_max(left_module_max_time);
			ship->get_right_module()->set_type(static_cast<Module::Type>(right_module));
			ship->get_right_module()->get_recharge_counter()->set(right_module_time);
			ship->get_right_module()->get_recharge_counter()->set_max(right_module_max_time);

			// Decoding commands
			std::vector<int> commands = aux::string_to_mask(commands_stringed);
			for (int i = 0; i < commands.size(); i++) {
				ship->get_player()->get_command_module()->set_command(i, commands[i]);
			}
			// Decoding effects
			std::vector<int> effects = aux::string_to_mask(effects_stringed);
			for (int i = 0; i < effects.size(); i++) {
				float val = effects[i];
				ship->get_effects()->get_effect((Effects::Types)i)->get_counter()->set(val);
			}
		}
		// Projectile
		if (symbol == "p") {
			//stream.get();
			int player_id = aux::read_int(stream);
			int number = aux::read_int8(stream);
			for (int i = 0; i < number; i++) {
				int id = aux::read_short(stream);
				b2Vec2 pos;
				pos.x = aux::read_float(stream, 2);
				pos.y = aux::read_float(stream, 2);
				float angle = aux::read_float(stream, 3);
				float radius = aux::read_float(stream, 2);
				// Creating projectile_def
				ProjectileDef projectile_def;
				projectile_def.pos = pos;
				projectile_def.radius = radius;
				projectile_def.player = players[player_id];
				// Creating projectile
				auto projectile = create_projectile(projectile_def);
				projectile->set_id(id);
				manage_destroyed_object(id);
			}
			//// Ids
			//stream.get();
			//int id, player_id;
			//id = aux::read_short(stream);
			//player_id = aux::read_int(stream);
			//// Pos
			//b2Vec2 pos;
			//pos.x = aux::read_float(stream, 2);
			//pos.y = aux::read_float(stream, 2);
			//// Angle
			//float angle;
			//angle = aux::read_float(stream, 3);
			//// Radius
			//float radius;
			//radius = aux::read_float(stream, 2);
			//// Creating projectile_def
			//ProjectileDef projectile_def;
			//projectile_def.pos = pos;
			//projectile_def.radius = radius;
			//projectile_def.player = players[player_id];
			//// Creating projectile
			//auto projectile = create_projectile(projectile_def);
			//projectile->set_id(id);
			//manage_destroyed_object(id);
		}
		// Rocket
		if (symbol == "r") {
			// Ids
			int id, player_id;
			// Pos
			b2Vec2 pos;
			// Angle
			float angle;
			// Radius
			float radius;
			//stream.get();
			id = aux::read_short(stream);
			player_id = aux::read_int(stream);
			pos.x = aux::read_float(stream, 2);
			pos.y = aux::read_float(stream, 2);
			angle = aux::read_float(stream, 3);
			radius = aux::read_float(stream, 2);
			// Creating rocket_def
			Rocket_Def rocket_def;
			rocket_def.pos = pos;
			rocket_def.base.radius = radius;
			rocket_def.angle = angle;
			rocket_def.player = players[player_id];
			// Createing rocket
			auto rocket = create_rocket(rocket_def);
			rocket->set_id(id);
			manage_destroyed_object(id);
		}
		// Bonus
		if (symbol == "b") {
			//stream.get();
			// Id
			int id = aux::read_int8(stream);
			// Pos
			b2Vec2 pos;
			pos.x = (aux::read_float(stream, 2));
			pos.y = aux::read_float(stream, 2);
			// Type
			int type = aux::read_int8(stream);
			// Bonus def
			Bonus_Def bonus_def;
			bonus_def.pos = pos;
			bonus_def.type = static_cast<Bonus::Types>(type);
			// Creating bonus
			auto bonus = create_bonus(bonus_def);
		}
		// Event
		if (symbol == "e") {
			int id;
			int type;
			b2Vec2 pos;
			//stream.get();
			id = aux::read_short(stream);
			type = aux::read_int8(stream);
			pos.x = aux::read_float(stream, 2);
			pos.y = aux::read_float(stream, 2);
			// TODO: find more elegant way of getting sound name
			std::map<int, std::string> sound_names;
			sound_names[Event::SHOT] = "shot";
			sound_names[Event::LASER] = "laser";
			sound_names[Event::WALL_HIT] = "hit";
			sound_names[Event::DEATH] = "death";
			sound_names[Event::BONUS_PICKUP] = "bonus_pickup";
			sound_names[Event::MODULE_SHOTGUN] = "shotgun";
			sound_names[Event::MODULE_FORCE] = "force";
			sound_names[Event::MODULE_BLINK] = "blink";
			sound_names[Event::MODULE_ROCKET] = "rocket";
			sound_names[Event::MODULE_DASH] = "dash";
			std::string sound_name = sound_names[type];

			audio->update_sound(id, sound_name, pos, 1, 0);
			// Creating event
			create_event({ type, nullptr, pos })->set_id(id);
			// Animations
			if (active_events.count(id))
				continue;
			if (type == Event::MODULE_FORCE) {
				draw->fadeout_animation("force",
					pos, // Position
					{ 0, 0.0 }, // Shift
					{ 0.3, module_manager.get_prototype(Module::FORCE)->params["radius"] }, // Size
					{ 0, 0 }, // Angle
					{ sf::Color::White, aux::make_transparent(sf::Color::White) }, // Color
					0.15, GAME // Duration, layer
				);
			}
			if (type == Event::MODULE_BLINK) {
				for (int i = 0; i < 10; i++)
					draw->fadeout_animation("bullet",
						pos, // Position
						{ 0.0, 0.3 }, // Shift
						{ 0.3, 0. }, // Size
						{ 0, 0 }, // Angle
						{ sf::Color::White, aux::make_transparent(sf::Color::White) }, // Color
						0.15, GAME // Duration, layer
					);
			}
			if (type == Event::MODULE_DASH || type == Event::WALL_HIT) {
				for (int i = 0; i < 10; i++)
					draw->fadeout_animation("bullet",
						pos, // Position
						{ 0.0, 0.3 }, // Shift
						{ 0.3, 0. }, // Size
						{ 0, 0 }, // Angle
						{ sf::Color::White, aux::make_transparent(sf::Color::White) }, // Color
						0.15, GAME // Duration, layer
					);
			}
			if (type == Event::DEATH) {
				for (int i = 0; i < 10; i++)
					draw->fadeout_animation("explosion",
						pos, // Position
						{ 0.0, 0.3 }, // Shift
						{ 0.3, 0.3 }, // Size
						{ 0, 0 }, // Angle
						{ sf::Color::White, aux::make_transparent(sf::Color::White) }, // Color
						0.15, GAME // Duration, layer
					);
			}
		}
	}

	// Damage animation
	if (get_ship(my_id) && get_ship(my_id)->get_hp()->get() < hp_prev) {
		// Animation
		FloatAnimation::State state_a;
		state_a.angle = 0;
		state_a.pos = { 0, 0 };
		state_a.scale = aux::to_b2Vec2(sf::Vector2f(draw->get_window()->getSize()));
		auto state_b = state_a;
		float min_alpha = 0.5;
		state_a.color.a = (min_alpha + ((hp_prev - get_ship(my_id)->get_hp()->get()) / get_ship(my_id)->get_hp()->get_max()) * (1 - min_alpha)) * 255;
		state_b.color.a = 0;
		draw->create_animation(FloatAnimation("blood", state_a, state_b, 1, HUD));
		// Sound
		audio->update_sound(aux::random_int(0, 1000), "damage", get_ship(my_id)->get_body()->GetPosition(), 1, 0);
	}

	// Managing disappeared objects
	for (auto object : destroyed_objects) {
		std::string sound_name = "";
		switch (object.second.type) {
		case Disappear_Animation::PROJECTILE:
			sound_name = "projectile_hit";
			break;
		case Disappear_Animation::ROCKET:
			sound_name = "explosion";
			break;
		}
		audio->update_sound(aux::random_int(0, 1000), sound_name, object.second.pos, 1, 0);

		// Animation
		switch (object.second.type) {
		case Disappear_Animation::PROJECTILE:
			for (int i = 0; i < 10; i++) {
				FloatAnimation::State state_begin;
				state_begin.pos = object.second.pos;
				state_begin.scale = object.second.radius * b2Vec2(4, 4);
				state_begin.angle = 0;
				state_begin.color = object.second.color;
				FloatAnimation::State state_end = state_begin;
				if (i != 0) {
					state_end.scale = b2Vec2_zero;
					state_end.pos += aux::rotate({ 0, 0.4 }, aux::random_float(0, 2, 2) * b2_pi);
				}
				else {
					state_begin.color = state_end.color = { 255, 255, 255 };
				}
				state_end.color.a = 0;
				FloatAnimation animation("bullet", state_begin, state_end, 0.1, GAME);
				draw->create_animation(animation);
			}
			break;
		case Disappear_Animation::ROCKET:
			for (int i = 0; i < 10; i++) {
				auto blast_radius = module_manager.get_prototype(Module::ROCKET)->params["blast_radius"];
				draw->fadeout_animation("explosion",
					object.second.pos, // Position
					{ 0.1, blast_radius / 4 }, // Shift
					{ 0.1, blast_radius / 2 }, // Size
					{ aux::random_float(0, 2 * b2_pi, 2), aux::random_float(0, 2 * b2_pi, 2) }, // Angle
					{ sf::Color::White, aux::make_transparent(sf::Color::White) }, // Color
					0.1, GAME // Duration, layer
				);
			}
			break;
		}
	}
}


void GameClient::load_setup(std::string path) {
	std::ifstream fileInput(path);
	std::stringstream file = (aux::comment(fileInput));

	std::string command; // Current command
	while (file >> command) {
		if (command == "END") // End of file
			break;
		if (command == "GUN") {
			file >> gun_name;
		}
		if (command == "HULL") {
			file >> hull_name;
		}
		if (command == "LEFT_MODULE") {
			file >> left_module_name;
		}
		if (command == "RIGHT_MODULE") {
			file >> right_module_name;
		}
	}
}

void GameClient::save_setup(std::string path) {
	std::ofstream file(path);
	file << "GUN " << gun_name << "\n" << "HULL " << hull_name << "\n"
		<< "LEFT_MODULE " << left_module_name << "\n" << "RIGHT_MODULE " << right_module_name;
	file.close();
}
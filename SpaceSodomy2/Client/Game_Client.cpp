#include "Game_Client.h"
#include <iostream>

void Game_Client::set_draw(Draw* _draw) {
	draw = _draw;
	draw->apply_camera({ 0, 0 }, 100, 0);
}
void Game_Client::set_audio(Audio* _audio) {
	audio = _audio;
}

Draw* Game_Client::get_draw() {
	return draw;
}
Audio* Game_Client::get_audio() {
	return audio;
}

void Game_Client::display(int id) {
	// Finding cam target
	auto ship = get_ship(id);
	if (ship) {
		draw->get_camera()->set_pos(ship->get_body()->GetPosition());
		draw->get_camera()->set_angle(ship->get_body()->GetAngle());
		draw->apply_camera();
	}

	// Clear scene
	draw->clear();

	// Background
	Camera camera_backup = *draw->get_camera();
	b2Vec2 mid = aux::to_b2Vec2(sf::Vector2f(draw->get_window()->getSize()));
	draw->apply_camera(b2Vec2(0, 0), mid.Normalize() / draw->get_texture("background")->getSize().x, camera_backup.get_angle());
	draw->image("background", b2Vec2(0, 0),
		aux::to_b2Vec2(sf::Vector2f(draw->get_texture("background")->getSize())), 0);
	draw->set_camera(camera_backup);
	draw->apply_camera();

	// Walls
	for (auto wall : walls) {
		auto vertices = wall->get_vertices();
		for (int i = 0; i < vertices.size(); i++) {
			int j = (i + 1) % vertices.size();
			draw->line(vertices[i], vertices[j], {255, 255, 255});
		}
	}

	// Ships
	for (auto ship : ships) {
		if (!ship->get_body() ||
			!ship->get_body()->GetFixtureList() ||
			!ship->get_body()->GetFixtureList()->GetShape())
			continue;

		// HP_bar & name
		b2Vec2* mouse_pos = new b2Vec2;
		aux::Keyboard* keyboard = new aux::Keyboard;
		Bar* HP_bar = new Bar();
		b2Vec2 pos = ship->get_body()->GetPosition() - draw->get_camera()->get_pos();
		pos.x *= draw->get_camera()->get_scale();
		pos.y *= draw->get_camera()->get_scale();
		HP_bar->set_value(ship->get_hp()->get());
		HP_bar->set_pos(pos + b2Vec2(0,10));
		HP_bar->set_scale(b2Vec2(50, 10));
		HP_bar->draw_text = 0;
		HP_bar->set_draw(draw);
		HP_bar->set_mouse_pos(mouse_pos);
		HP_bar->set_angle((draw->get_camera()->get_angle() + b2_pi / 2) / b2_pi * 180);
		HP_bar->set_keyboard(keyboard);
		Constant_Text* name = new Constant_Text();
		name->set_color(ship->get_player()->get_color());
		name->set_text(ship->get_player()->get_name());
		name->set_pos(pos + b2Vec2(0, 20));
		name->set_text_character_pixel_size(30);
		name->set_text_scale(1);
		name->set_draw(draw);
		name->set_mouse_pos(mouse_pos);
		name->set_keyboard(keyboard);
		name->set_text_angle((draw->get_camera()->get_angle() + b2_pi / 2) / b2_pi * 180);

		float radius = ship->get_body()->GetFixtureList()->GetShape()->m_radius * 2;
		auto color = ship->get_player()->get_color();
		draw->image("ship", ship->get_body()->GetPosition(), {radius, radius}, ship->get_body()->GetAngle());
		draw->image("ship_colors", ship->get_body()->GetPosition(), {radius, radius}, 
			ship->get_body()->GetAngle(), color);

		Camera camera_backup = *draw->get_camera();
		draw->apply_camera(b2Vec2(0, 0), 1, camera_backup.get_angle());
		HP_bar->step();
		name->step();
		draw->set_camera(camera_backup);
		draw->apply_camera();
		// Engines
		std::vector<std::string> textures = {
			"engine_lin_forward",
			"engine_lin_backward",
			"engine_lin_left",
			"engine_lin_right",
			"engine_turn_left",
			"engine_turn_right"
		};
		radius *= 2;
		for (int i = 0; i < textures.size(); i++) {
			if (ship->get_player()->get_command_module()->get_command(i))
				draw->image(textures[i], ship->get_body()->GetPosition(),
					{ radius, radius }, ship->get_body()->GetAngle(), color);
		}
		delete mouse_pos;
		delete keyboard;
		delete HP_bar;
		delete name;
	}

	// Projectiles
	for (auto projectile : projectiles) {
		float radius = projectile->get_body()->GetFixtureList()->GetShape()->m_radius * 2;
		auto color = projectile->get_player()->get_color();
		draw->image("bullet", projectile->get_body()->GetPosition(), { radius, radius }, 
			projectile->get_body()->GetAngle(), color);
		radius *= 0.8;
		draw->image("bullet", projectile->get_body()->GetPosition(), { radius, radius },
			projectile->get_body()->GetAngle());

	}
}

void Game_Client::decode(std::string source) {
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
		// Player
		if (symbol == "P") {
			// Id
			int id;
			stream >> id;
			// Color
			sf::Color color;
			int r, g, b;
			stream >> r >> g >> b;
			color = sf::Color(r, g, b);
			// Name
			std::string name;
			stream >> name;
			// Creating player
			Player* player = create_player(id);
			player->set_color(color);
			player->set_name(name);
		}
		// Ship
		if (symbol == "S") {
			// Ids
			int id, player_id;
			stream >> id >> player_id;
			// Pos
			b2Vec2 pos;
			stream >> pos.x >> pos.y;
			// Angle
			float angle;
			stream >> angle;
			// Commands
			std::string commands_stringed;
			stream >> commands_stringed;
			// Hp
			float hp;
			stream >> hp;
			if (hp < 50) {
				std::cout << "hp: " << hp << "\n";
			}
			float stamina;
			stream >> stamina;
			std::cout << "stamina: " << stamina << "\n";

			auto ship = create_ship(players[player_id], pos, angle);
			ship->get_hp()->set(hp);
			ship->get_stamina()->set(stamina);

			// Decoding commands
			std::vector<int> commands = aux::string_to_mask(commands_stringed);
			for (int i = 0; i < commands.size(); i++)
				ship->get_player()->get_command_module()->set_command(i, commands[i]);
		}
		// Projectile
		if (symbol == "p") {
			// Ids
			int id, player_id;
			stream >> id >> player_id;
			// Pos
			b2Vec2 pos;
			stream >> pos.x >> pos.y;
			// Angle
			float angle;
			stream >> angle;
			// Creating projectile_def
			Projectile_Def projectile_def;
			projectile_def.pos = pos;
			projectile_def.player = players[player_id];
			// Createing projectile
			auto projectile = create_projectile(projectile_def);
		}
		// Event
		if (symbol == "e") {
			int id;
			std::string name;
			b2Vec2 pos;
			
			stream >> id >> name >> pos.x >> pos.y;

			audio->update_sound(id, name, pos);
		}
	}
}

Ship* Game_Client::get_ship(int id) {
	for (auto ship : ships)
		if (ship->get_player() == players[id])
			return ship;
}
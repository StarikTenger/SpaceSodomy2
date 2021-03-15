#include "Game_Client.h"
#include <iostream>

void Game_Client::set_draw(Draw* _draw) {
	draw = _draw;
	draw->apply_camera({ 0, 0 }, 100, 0);
}

Draw* Game_Client::get_draw() {
	return draw;
}

void Game_Client::display(int id) {
	//draw->apply_camera({0, 0}, 100, 0);
	// Finding cam target
	for (auto ship : ships) {
		if (ship->get_player()->get_id() == id) {
			draw->get_camera()->set_pos(ship->get_body()->GetPosition());
			draw->get_camera()->set_angle(ship->get_body()->GetAngle());
		}
	}
	draw->apply_camera();

	// Clear scene
	draw->clear();

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
		float radius = ship->get_body()->GetFixtureList()->GetShape()->m_radius * 2;
		draw->image("ship", ship->get_body()->GetPosition(), {radius, radius}, ship->get_body()->GetAngle());
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
					{ radius, radius }, ship->get_body()->GetAngle());
		}

	}

	// Projectiles
	for (auto projectile : projectiles) {
		float radius = projectile->get_body()->GetFixtureList()->GetShape()->m_radius * 2;
		draw->image("bullet", projectile->get_body()->GetPosition(), { radius, radius }, projectile->get_body()->GetAngle());

	}
}

void Game_Client::decode(std::string source) {
	std::cout << source << "\n";

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
			int id;
			stream >> id;
			sf::Color color;
			int r, g, b;
			stream >> r >> g >> b;
			color = sf::Color(r, g, b);
			std::string name;
			stream >> name;
			Player* player = create_player(id);
			player->set_color(color);
			player->set_name(name);
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
			//auto ship = new_player(player_id, { 255, 0, 0 }, "_name_", pos, angle);
			//if (players.at(player_id))
			auto ship = create_ship(players[player_id], pos, angle);

			for (int i = 0; i < commands.size(); i++)
				ship->get_player()->get_command_module()->set_command(i, commands[i]);
		}
		// Projectile
		if (symbol == "p") {
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
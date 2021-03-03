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
			if (ship->get_command_module()->get_command(i))
				draw->image(textures[i], ship->get_body()->GetPosition(), 
					{ radius, radius }, ship->get_body()->GetAngle());
		}

	}

	// Projectiles
	for (auto projectile : projectiles) {
		float radius = projectile->get_body()->GetFixtureList()->GetShape()->m_radius * 2;
		draw->image("bullet", projectile->get_body()->GetPosition(), { radius, radius }, projectile->get_body()->GetAngle());

	}
	
	draw->display();
}
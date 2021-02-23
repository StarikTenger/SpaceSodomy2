#include "Game_Client.h"
#include <iostream>

void Game_Client::set_draw(Draw* _draw) {
	draw = _draw;
}

void Game_Client::display() {
	draw->apply_camera({0, 0}, 100, 0);
	draw->clear();

	// Walls
	for (auto wall : walls) {
		auto verticies = wall->get_verticies();
		for (int i = 0; i < verticies.size(); i++) {
			int j = (i + 1) % verticies.size();
			draw->line(verticies[i], verticies[j], {255, 255, 255});
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
	}
	
	draw->display();
}
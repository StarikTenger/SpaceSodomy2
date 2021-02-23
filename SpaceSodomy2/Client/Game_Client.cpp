#include "Game_Client.h"
#include <iostream>

void Game_Client::set_draw(Draw* _draw) {
	draw = _draw;
}

void Game_Client::display() {
	draw->apply_camera({0, 0}, 100, 0);
	draw->clear();

	for (auto ship : ships) {
		if (!ship->get_body() ||
			!ship->get_body()->GetFixtureList() ||
			!ship->get_body()->GetFixtureList()->GetShape())
			continue;
		float radius = ship->get_body()->GetFixtureList()->GetShape()->m_radius;
		draw->image("ship", ship->get_body()->GetPosition(), {radius, radius}, ship->get_body()->GetAngle());
	}
	
	draw->display();
}
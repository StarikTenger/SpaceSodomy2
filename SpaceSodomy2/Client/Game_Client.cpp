#include "Game_Client.h"
#include <iostream>

void Game_Client::set_draw(Draw* _draw) {
	draw = _draw;
}

void Game_Client::display() {
	for (auto ship : ships) {
		if (!ship->get_body() ||
			!ship->get_body()->GetFixtureList() ||
			!ship->get_body()->GetFixtureList()->GetShape())
			continue;
		float radius = 5;// ship->get_body()->GetFixtureList()->GetShape()->m_radius;
		draw->fill_circle(10*ship->get_body()->GetPosition(), radius, {255, 255, 255});
	}

	draw->display();
}
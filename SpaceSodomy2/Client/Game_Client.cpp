#include "Game_Client.h"
#include <iostream>

void Game_Client::set_draw(Draw* _draw) {
	draw = _draw;
}

void Game_Client::display() {
	draw->fill_rect({ 0, 0 }, { 10000, 10000 }, {0, 0 ,0});

	for (auto ship : ships) {
		if (!ship->get_body() ||
			!ship->get_body()->GetFixtureList() ||
			!ship->get_body()->GetFixtureList()->GetShape())
			continue;
		float radius = 50;// ship->get_body()->GetFixtureList()->GetShape()->m_radius;
		//draw->fill_circle(10*ship->get_body()->GetPosition(), radius, {255, 255, 255});
		draw->image("ship", 100*ship->get_body()->GetPosition(), {radius, radius}, ship->get_body()->GetAngle());
	}
	
	draw->display();
}
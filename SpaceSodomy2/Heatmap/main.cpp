#include <Draw/Draw.h>
#include <iostream>
#include "Grid.h"

int main() {
	// From input
	std::string path = "gay.pidor";
	std::ifstream file(path);
	std::vector<b2Vec2> points;
	int count = 0;
	while (!file.eof()) {
		b2Vec2 cur;
		file >> cur.x;
		file >> cur.y;
		points.push_back(cur);
		if (count % 1000 == 0) {
			std::cout << count << "\n";
		}
		count++;
	}
	// Building grid
	std::vector<Grid> grids;
	grids.push_back(Grid({ 40, 40 }, points, { -20, -20 }, 0.2));
	grids.push_back(Grid({ 40, 40 }, points, { -20.1, -20.1 }, 0.2));
	

	// Displaying
	Draw draw;
	Camera cam;
	cam.set_scale(10);
	cam.set_pos({ 0, 0 });
	draw.set_camera(cam);
	
	draw.create_window(600, 600, "Heatmap");
	b2Vec2 mouse_prev = b2Vec2_zero;
	while (draw.get_window()->isOpen()) {
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		float delta = 0;
		while (draw.get_window()->pollEvent(event)) {
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				draw.get_window()->close();
			if (event.type == sf::Event::MouseWheelMoved) {
				delta = event.mouseWheel.delta;
			}
		}
		auto pos = aux::to_b2Vec2(sf::Vector2f(sf::Mouse::getPosition()));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			draw.move_camera(aux::rotate(1. / draw.get_camera()->get_scale() * (pos - mouse_prev),
				draw.get_camera()->get_angle() - b2_pi/2));
		}
		float dS = pow(1.1, delta);
		draw.get_camera()->modify_scale(dS);
		draw.get_camera()->set_borders(aux::to_b2Vec2(sf::Vector2f(draw.get_window()->getSize())));
		float scale = draw.get_camera()->get_scale();
		draw.move_camera(aux::rotate(-1. / scale * (1 - dS) * (0.5 * draw.get_camera()->get_borders() - pos),
			-b2_pi / 2));
		mouse_prev = pos;
		draw.clear();
		draw.apply_camera();
		for (auto& grid : grids) {
			grid.display(draw);
		}
		draw.display();
	}
}
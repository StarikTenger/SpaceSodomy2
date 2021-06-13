#include <Draw/Draw.h>
#include <Game/Game.h>
#include <iostream>
#include "Grid.h"

class Game_Drawable : public Game {
public:
	void draw_walls(Draw* draw) {
		for (auto wall : walls) {
			auto color = sf::Color(0, 151, 255);
			if (wall->get_type() == Wall::SPIKED || wall->get_type() == Wall::GHOST) {
				color = sf::Color(255, 255, 255);
			}
			auto vertices = wall->get_vertices();
			for (int i = 0; i < vertices.size(); i++) {
				int j = (i + 1) % vertices.size();
				float thickness = 0.05;
				draw->thick_line(vertices[i], vertices[j], color, thickness);

				// Drawing wall connection
				b2Vec2 vec_a = vertices[(i + 1) % vertices.size()] - vertices[i];
				b2Vec2 vec_b = vertices[(vertices.size() + i - 1) % vertices.size()] - vertices[i];
				vec_a.Normalize();
				vec_b.Normalize();
				b2Vec2 vec_dir = vec_a + vec_b;
				vec_dir.Normalize();
				float cos_val = b2Dot(vec_a, vec_b);
				float sin_val = sqrt(abs(1 - cos_val) / 2);
				cos_val = sqrt(abs(1 + cos_val) / 2);
				float size_x = thickness * sin_val / 2;
				float size_y = thickness * cos_val;
				draw->thick_line(vertices[i] - size_x * vec_dir, vertices[i] + size_x * vec_dir, color, size_y);
			}
		}
	}

};

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
	Game_Drawable game;
	game.load_map("maps/rocks.lvl");

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
		game.draw_walls(&draw);
		draw.display();
	}
}
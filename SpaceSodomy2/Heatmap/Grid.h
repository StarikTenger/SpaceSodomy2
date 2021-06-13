#pragma once
#include <box2d/box2d.h>
#include <Draw/Draw.h>
#include <vector>
#include <tuple>
#include <stdexcept>

class Grid {
private:
	b2Vec2 offset = b2Vec2_zero;
	float cell_size = 1;
	std::vector<std::vector<float>> data;
	std::tuple<bool, sf::Vector2i> transform(b2Vec2 vec);
public:
	Grid();
	Grid(b2Vec2 size, const std::vector<b2Vec2>& points, b2Vec2 _offset, float _cell_size);

	void display(Draw& draw, float scale = 1.0);
};


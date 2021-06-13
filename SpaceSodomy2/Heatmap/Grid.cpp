#include "Grid.h"

std::tuple<bool, sf::Vector2i> Grid::transform(b2Vec2 vec) {
	vec -= offset;
	vec *= 1. / cell_size;
	sf::Vector2i pos((int)vec.x, (int)vec.y);
	bool in_bounds = !(pos.x < 0 || pos.y < 0 || pos.x >= data.size() || pos.y >= data[0].size());
	return std::make_tuple(in_bounds, pos);
} 

Grid::Grid() {
}

Grid::Grid(b2Vec2 size, const std::vector<b2Vec2>& points, b2Vec2 _offset, float _cell_size) {
	offset = _offset;
	cell_size = _cell_size;
	size *= 1. / cell_size;
	// Create data
	data = {};
	for (int x = 0; x < size.x; x++) {
		data.push_back({});
		for (int y = 0; y < size.y; y++) {
			data[x].push_back(0);
		}
	}
	// Apply points
	for (auto p : points) {
		bool in_bounds;
		sf::Vector2i cell_pos;
		std::tie(in_bounds, cell_pos) = transform(p);
		if (!in_bounds)
			continue;
		data[cell_pos.x][cell_pos.y] += 1;
	}
}

void Grid::display(Draw& draw, float scale) {
	float max_val = -1e9;
	for (int x = 0; x < data.size(); x++)
		for (int y = 0; y < data[x].size(); y++)
			if (data[x][y] > max_val)
				max_val = data[x][y];

	for (int x = 0; x < data.size(); x++) {
		for (int y = 0; y < data[x].size(); y++) {
			sf::Color col = {255, 255, 255};
			col.a = 255 * data[x][y] / max_val;
			draw.fill_rect(scale * (b2Vec2(x, y) + offset), scale * b2Vec2(1, 1), col);
		}
	}
	
}

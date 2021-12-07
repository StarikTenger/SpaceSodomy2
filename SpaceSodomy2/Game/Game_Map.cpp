#include "pch.h"
#include "Game_Map.h"

void Game_Map::insert_wall(Wall* ptr) {
    walls.insert(ptr);
	if (ptr->get_orientation() == Wall::INNER) {
		for (auto point : ptr->get_vertices()) {
			lower_left_corner.x = std::min(
				lower_left_corner.x,
				point.x);
			lower_left_corner.y = std::min(
				lower_left_corner.y,
				point.y);
			upper_right_corner.x = std::max(
				upper_right_corner.x,
				point.x);
			upper_right_corner.y = std::max(
				upper_right_corner.y,
				point.y);
		}
	}

}
bool Game_Map::is_coordinate_allowed(b2Vec2 vec) {
	bool res = true;
	for (auto wall : walls) {
		res &= !aux::is_in_polygon(vec, wall->get_vertices(), wall->get_orientation());
		res &= (aux::dist_from_polygon(vec, wall->get_vertices()) > 1);      // TODO
		if (!res) {
			return false;
		}
	}
	return true;
}
void Game_Map::clear() {
	walls.clear();
	lower_left_corner = { 10000, 10000 };
	upper_right_corner = { -10000, -10000 };

}
b2Vec2 Game_Map::get_random_coord() {
	bool acceptable = false;
	b2Vec2 res;
	while (!acceptable) {
		res = b2Vec2(aux::random_int(lower_left_corner.x, upper_right_corner.x) + aux::random_float(-1, 1, 4),
			aux::random_int(lower_left_corner.y, upper_right_corner.y) + aux::random_float(-1, 1, 4));
		acceptable = is_coordinate_allowed(res);
	}
	return res;
}
b2Vec2 Game_Map::get_beam_intersection(b2Vec2 start, float angle) {
	b2Vec2 closest_intersection;
	float closest_distance = 1e9;
	b2Vec2 finish = start + 1e3 * aux::angle_to_vec(angle);
	for (auto wall : walls) {
		auto polygon = wall->get_vertices();
		for (int i = 0; i < polygon.size(); i++) {
			int j = (i + 1) % polygon.size();
			auto intersection = aux::segment_intersection({ start, finish }, { polygon[i], polygon[j] });
			if (intersection.first) {
				float distance = b2Distance(start, intersection.second);
				if (distance < closest_distance) {
					closest_distance = distance;
					closest_intersection = intersection.second;
				}
			}
		}
	}
	return closest_intersection;
}
std::set<Wall*>* Game_Map::get_walls() {
	return &walls;
}






#include "pch.h"
#include "Wall.h"

Wall::Wall() {

}

std::vector<b2Vec2> Wall::get_vertices() {
	return vertices;
}

b2Body* Wall::get_body() {
	return body;
}

int Wall::get_orientation() {
	return orientation;
}

void Wall::set(b2World* world, std::vector<b2Vec2> _vertices, int _orientation) {
	vertices = _vertices;
	// Check for orientation
	float area = aux::area(vertices);
	if (area * (_orientation == OUTER ? 1 : -1) < 0)
		std::reverse(vertices.begin(), vertices.end());
	orientation = _orientation;

	if (!body) {
		b2BodyDef body_def;
		body_def.position.Set(0.0f, 0.0f);
		body = world->CreateBody(&body_def);
	}
	b2ChainShape shape;
	shape.CreateLoop(vertices.data(), vertices.size());
	body->CreateFixture(&shape, 0);
	body->GetFixtureList()->SetFriction(0);
	body->GetFixtureList()->SetRestitutionThreshold(0);
	body->GetFixtureList()->SetRestitution(0.5);
}

/*void Wall::set_orientation(int val) {
	orientation = val;
}*/

int Wall::hash() {
	return vertices[0].x * 31415 + vertices[0].y * 11;
}

void Wall::init_draw() {
	if (get_orientation()) {
		origin_pos = aux::origin_pos(vertices);
		box_size = aux::box_size(vertices);
	}
	else {
		box_size = aux::mult(aux::box_size(vertices), outer_bound_of_inner_wall_textures);
		origin_pos = aux::origin_pos(vertices) - aux::mult(aux::box_size(vertices), outer_bound_of_inner_wall_textures / 2);
	}
}

b2Vec2 Wall::get_origin_pos() {
	return origin_pos;
}
b2Vec2 Wall::get_box_size() {
	return box_size;
}

int Wall::get_outer_bound_of_inner_wall_textures() {
	return outer_bound_of_inner_wall_textures;
}

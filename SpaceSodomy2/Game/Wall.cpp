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

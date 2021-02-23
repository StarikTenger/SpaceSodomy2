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

void Wall::set(b2World* world, std::vector<b2Vec2> _vertices) {
	vertices = _vertices;
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
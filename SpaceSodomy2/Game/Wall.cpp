#include "pch.h"
#include "Wall.h"

Wall::Wall() {

}

std::vector<b2Vec2> Wall::get_verticies() {
	return verticies;
}

b2Body* Wall::get_body() {
	return body;
}

void Wall::set(b2World* world, std::vector<b2Vec2> _verticies) {
	verticies = _verticies;
	if (!body) {
		b2BodyDef body_def;
		body_def.position.Set(0.0f, 0.0f);
		body = world->CreateBody(&body_def);
	}
	b2PolygonShape shape;
	shape.Set(_verticies.data(), _verticies.size());
	body->CreateFixture(&shape, 0);
}
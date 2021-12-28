#include "pch.h"
#include "Forcefield.h"

std::vector<b2Vec2> Forcefield::get_vertices() {
	return vertices;
}

b2Body* Forcefield::get_body() {
	return body;
}

b2Vec2 Forcefield::get_force() {
	return force;
}

void Forcefield::set(b2World* world, std::vector<b2Vec2> _vertices, b2Vec2 force_) {
	vertices = _vertices;
	if (!body) {
		b2BodyDef body_def;
		body_def.position.Set(0.0f, 0.0f);
		body = world->CreateBody(&body_def);
	}
	force = force_;
	b2ChainShape shape;
	shape.CreateLoop(vertices.data(), vertices.size());
	body->CreateFixture(&shape, 0);
	body->GetFixtureList()->SetFriction(0);
	body->GetFixtureList()->SetRestitutionThreshold(0);
	body->GetFixtureList()->SetRestitution(0.5);
}

void Forcefield::init_drawing(float Forcefield_width) {

	origin_pos = aux::origin_pos(vertices) - b2Vec2(Forcefield_width, Forcefield_width);
	box_size = aux::box_size(vertices) + b2Vec2(Forcefield_width * 2.f, Forcefield_width * 2.f);

}

b2Vec2 Forcefield::get_origin_pos() {
	return origin_pos;
}
b2Vec2 Forcefield::get_box_size() {
	return box_size;
}

bool Forcefield::apply(b2Body* body, float dt) {
	if (aux::is_in_polygon(body->GetWorldPoint({ 0,0 }), vertices, 1)) {
		body->ApplyLinearImpulseToCenter(dt * body->GetMass() * force, true);
		return true;
	}
	else {
		return false;
	}
}
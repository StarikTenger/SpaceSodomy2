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


	origin_pos = aux::origin_pos(vertices);
	box_size = aux::box_size(vertices);
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

std::vector<Forcefield::Point> Forcefield::get_pts_for_grid(float step) {
	std::vector<Point> res;
	for (float x = 0; x < box_size.x; x += step) {
		for (float y = 0; y < box_size.y; y += step) {
			b2Vec2 pos = origin_pos + b2Vec2{x, y};
			if (aux::is_in_polygon(pos, vertices, true)) {
				res.push_back({ force, pos });
			}
		}
	}
	return res;
}
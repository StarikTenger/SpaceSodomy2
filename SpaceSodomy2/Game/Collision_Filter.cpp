#include "pch.h"
#include "Collision_Filter.h"

int Collision_Filter::get_type(b2Fixture* fixture) {
	if (collision_table.find(fixture->GetBody()) == collision_table.end())
		return STANDART;
	return collision_table[fixture->GetBody()];
}

Collision_Filter::Collision_Filter() {
}

void Collision_Filter::add_body(b2Body* body, int type) {
	collision_table.insert({ body, type });
}

bool Collision_Filter::ShouldCollide(b2Fixture* fixture_a, b2Fixture* fixture_b) {
	int type_a = get_type(fixture_a);
	int type_b = get_type(fixture_b);
	for (auto p : collide_pairs) {
		if (type_a == p.first && type_b == p.second ||
			type_b == p.first && type_a == p.second)
			return true;
	}

	return false;
}
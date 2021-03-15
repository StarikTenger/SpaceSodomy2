#include "pch.h"
#include "Collision_Filter.h"

int Collision_Filter::get_type(b2Fixture* fixture) {
	if (collision_table.find(fixture->GetBody()) == collision_table.end())
		return STANDART;
	return collision_table[fixture->GetBody()].type;
}

int Collision_Filter::get_id(b2Fixture* fixture) {
	if (collision_table.find(fixture->GetBody()) == collision_table.end())
		return 0;
	return collision_table[fixture->GetBody()].id;
}

Collision_Filter::Collision_Filter() {
}

void Collision_Filter::add_body(b2Body* body, int type, int id) {
	collision_table.insert({ body, {type, id} });
}

void Collision_Filter::delete_body(b2Body* body) {
	collision_table.erase(body);
}

bool Collision_Filter::ShouldCollide(b2Fixture* fixture_a, b2Fixture* fixture_b) {
	int type_a = get_type(fixture_a);
	int type_b = get_type(fixture_b);
	int id_a = get_id(fixture_a);
	int id_b = get_id(fixture_b);

	for (auto p : collide_pairs) {
		if ((type_a == p[0] && type_b == p[1] ||
			type_b == p[0] && type_a == p[1]) 
			&&
			((id_a == id_b) && p[2] == 1 ||
			(id_a != id_b) && p[2] == -1 ||
			p[2] == 0))
			return true;
	}

	return false;
}
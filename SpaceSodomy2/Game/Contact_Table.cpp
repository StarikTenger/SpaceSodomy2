#include "pch.h"
#include "ContactTable.h"

ContactTable::ContactTable() {
}

void ContactTable::set_collision_filter(CollisionFilter* val) {
	collision_filter = val;
}

void ContactTable::reset() {
	table.clear();
}

void ContactTable::add(b2Body* body_a, b2Body* body_b) {
	table.insert({ body_a, body_b });
}

bool ContactTable::check(b2Body* body_a, b2Body* body_b) {
	return table.find({ body_a, body_b }) != table.end() || 
		table.find({ body_b, body_a }) != table.end();
}

void ContactTable::BeginContact(b2Contact* contact) {
	/* handle begin event */
}

void ContactTable::EndContact(b2Contact* contact){
	/* handle end event */
}

void ContactTable::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	contact->SetEnabled(0);
	if (contact->IsTouching()) {
		add(contact->GetFixtureA()->GetBody(),
			contact->GetFixtureB()->GetBody());
		if (collision_filter->ShouldCollide(contact->GetFixtureA(), contact->GetFixtureB()))
			contact->SetEnabled(1);			
	}
}

void ContactTable::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
	/* handle post-solve event */
}
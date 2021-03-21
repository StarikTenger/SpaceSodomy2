#include "pch.h"
#include "Contact_Table.h"

Contact_Table::Contact_Table() {
}

void Contact_Table::set_collision_filter(Collision_Filter* val) {
	collision_filter = val;
}

void Contact_Table::reset() {
	table.clear();
}

void Contact_Table::add(b2Body* body_a, b2Body* body_b) {
	table.insert({ body_a, body_b });
}

bool Contact_Table::check(b2Body* body_a, b2Body* body_b) {
	return table.find({ body_a, body_b }) != table.end() || 
		table.find({ body_b, body_a }) != table.end();
}

void Contact_Table::BeginContact(b2Contact* contact) {
	/* handle begin event */
}

void Contact_Table::EndContact(b2Contact* contact){
	/* handle end event */
}

void Contact_Table::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	contact->SetEnabled(0);
	if (contact->IsTouching()) {
		add(contact->GetFixtureA()->GetBody(),
			contact->GetFixtureB()->GetBody());
		if (collision_filter->ShouldCollide(contact->GetFixtureA(), contact->GetFixtureB()))
			contact->SetEnabled(1);			
	}
}

void Contact_Table::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
	/* handle post-solve event */
}
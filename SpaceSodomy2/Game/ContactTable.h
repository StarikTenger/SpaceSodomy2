#pragma once
#include <box2d/box2d.h>
#include <set>
#include "CollisionFilter.h"

class ContactTable : public b2ContactListener {
private:
	std::set<std::pair<b2Body*, b2Body*>> table;
	CollisionFilter* collision_filter = nullptr;

public:
	ContactTable();

	void set_collision_filter(CollisionFilter*);

	void reset();
	void add(b2Body*, b2Body*);
	bool check(b2Body*, b2Body*);

	void BeginContact(b2Contact* contact) override;

	void EndContact(b2Contact* contact) override;

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override;

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override;
};


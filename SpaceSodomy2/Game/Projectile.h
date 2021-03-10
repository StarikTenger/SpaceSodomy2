#pragma once
#include <box2d/box2d.h>
#include "Player.h"
#include "iId.h"

class Projectile : public iId {
private:
	// Related entities
	b2Body* body = nullptr;
	Player* player = nullptr;

	// Some characteristics
	float damage = 0;

public:
	// Constructor
	Projectile();

	// Get methods
	b2Body* get_body();
	Player* get_player();
	float get_damage();

	// Set methods
	void set_body(b2Body*);
	void set_player(Player*);
	void set_damage(float);
};


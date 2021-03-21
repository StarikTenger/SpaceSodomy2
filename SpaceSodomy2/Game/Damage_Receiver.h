#pragma once
#include <box2d/box2d.h>
#include "Counter.h"
#include "Player.h"
#include "iId.h"

class Damage_Receiver : public iId {
private:
	b2Body* body = nullptr;
	Counter* hp = nullptr;
	Player* last_hit = nullptr;
public:

	// Constructor
	Damage_Receiver(b2Body* _body = nullptr, Counter* _hp = nullptr);

	// Get methods
	b2Body* get_body();
	Counter* get_hp();
	Player* get_last_hit();

	// Set methods
	void set_body(b2Body*);
	void set_hp(Counter*);

	// Deals damage
	void damage(float value, Player* hit_by);
};


#pragma once
#include <box2d/box2d.h>
#include "Counter.h"
#include "Player.h"
#include "iId.h"
#include "Effects.h"

class Damage_Receiver : public iId {
private:
	b2Body* body = nullptr;
	Counter* hp = nullptr;
	Player* last_hit = nullptr;
	Player* player = nullptr;
	Effects* effects = nullptr;
public:

	// Constructor
	Damage_Receiver(b2Body* _body = nullptr, Counter* _hp = nullptr);

	// Get methods
	b2Body* get_body();
	Counter* get_hp();
	Player* get_last_hit();
	Player* get_player();
	Effects* get_effects();

	// Set methods
	void set_body(b2Body*);
	void set_hp(Counter*);
	void set_player(Player*);
	void set_effects(Effects*);


	// Deals damage
	void damage(float value, Player* hit_by);
};


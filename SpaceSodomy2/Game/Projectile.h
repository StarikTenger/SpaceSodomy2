#pragma once
#include <box2d/box2d.h>
#include "DamageReceiver.h"
#include "iId.h"


class Projectile : public iId {
private:
	// Related entities
	b2Body* body = nullptr;
	Player* player = nullptr;
	Counter* hp = nullptr;
	DamageReceiver* damage_receiver = nullptr;
	Effects_Prototype* effects_prototype = nullptr;

	// Some characteristics
	float damage = 0;

public:
	// Constructor
	Projectile();

	// Get methods
	b2Body* get_body();
	Player* get_player();
	Counter* get_hp();
	DamageReceiver* get_damage_receiver();
	Effects_Prototype* get_effects_def();
	float get_damage();

	// Set methods
	void set_body(b2Body*);
	void set_player(Player*);
	void set_hp(Counter*);
	void set_damage_receiver(DamageReceiver*);
	void set_damage(float);
	void set_effects_prototype(Effects_Prototype*);
};


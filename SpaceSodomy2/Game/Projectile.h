#pragma once
#include <box2d/box2d.h>
#include "Player.h"
#include "Damage_Receiver.h"
#include "iId.h"

class Projectile : public iId {
private:
	// Related entities
	b2Body* body = nullptr;
	Player* player = nullptr;
	Counter* hp = nullptr;
	Damage_Receiver* damage_receiver = nullptr;

	// Some characteristics
	float damage = 0;

public:
	// Constructor
	Projectile();

	// Get methods
	b2Body* get_body();
	Player* get_player();
	Counter* get_hp();
	Damage_Receiver* get_damage_receiver();
	float get_damage();

	// Set methods
	void set_body(b2Body*);
	void set_player(Player*);
	void set_hp(Counter*);
	void set_damage_receiver(Damage_Receiver*);
	void set_damage(float);
};


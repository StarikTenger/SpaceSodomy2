#pragma once
#include <box2d/box2d.h>
#include "Player.h"
#include "Damage_Receiver.h"
#include "iId.h"

class Projectile : public iId {
private:
	// Related entities
	b2Body* body = nullptr;
	Combatant* player = nullptr;
	Counter* hp = nullptr;
	Damage_Receiver* damage_receiver = nullptr;
	Effects_Prototype* effects_prototype = nullptr;

	// Some characteristics
	float damage = 0;

public:
	// Constructor
	Projectile();

	// Get methods
	b2Body* get_body();
	Combatant* get_combatant();
	Counter* get_hp();
	Damage_Receiver* get_damage_receiver();
	Effects_Prototype* get_effects_def();
	float get_damage();

	// Set methods
	void set_body(b2Body*);
	void set_combatant(Combatant*);
	void set_hp(Counter*);
	void set_damage_receiver(Damage_Receiver*);
	void set_damage(float);
	void set_effects_prototype(Effects_Prototype*);
};


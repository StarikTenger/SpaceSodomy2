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
	Combatant* last_hit = nullptr;
	Combatant* player = nullptr;
	Effects* effects = nullptr;
	float imm_frames = 0.f;
public:

	// Constructor
	Damage_Receiver(b2Body* _body = nullptr, Counter* _hp = nullptr);

	// Get methods
	b2Body* get_body();
	Counter* get_hp();
	Combatant* get_last_hit();
	Combatant* get_combatant();
	Effects* get_effects();
	float get_imm_frames();

	// Set methods
	void set_body(b2Body*);
	void set_hp(Counter*);
	void set_combatant(Combatant*);
	void set_effects(Effects*);
	void set_imm_frames(float);


	// Deals damage
	void damage(float value, Combatant* hit_by);
	// Apply effects. Returns false if impossible
	bool apply_effects(Effects_Prototype*);
	bool add_effect(Effects::Types, float val);
};


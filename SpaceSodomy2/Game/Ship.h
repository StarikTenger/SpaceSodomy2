#pragma once
#include <box2d/box2d.h>
#include "Engine.h"
#include "Player.h"
#include "Gun.h"
#include "Counter.h"
#include "DamageReceiver.h"
#include "BonusSlot.h"
#include "iId.h"
#include "Modules.h"

class Module;

class Ship : public iId{
private:
	// Entities
	Player* player = nullptr;
	Engine* engine = nullptr;
	b2Body* body = nullptr;
	Gun* gun = nullptr;
	Module* left_module = nullptr;
	Module* right_module = nullptr;
	Counter* hp = nullptr;
	Counter* stamina = nullptr;
	Counter* energy = nullptr;
	DamageReceiver* damage_receiver = nullptr;
	Effects* effects;
	BonusSlot* bonus_slot;

public:
	Ship();
	~Ship();

	// Get methods
	Player* get_player();
	Engine* get_engine();
	b2Body* get_body();
	Gun* get_gun();
	Counter* get_hp();
	Counter* get_stamina();
	Counter* get_energy();
	DamageReceiver* get_damage_receiver();
	Module* get_left_module();
	Module* get_right_module();
	Effects* get_effects();
	BonusSlot* get_bonus_slot();

	// Set methods
	void set_player(Player*);
	void set_engine(Engine*);
	void set_body(b2Body*);
	void set_gun(Gun*);
	void set_hp(Counter*);
	void set_stamina(Counter*);
	void set_energy(Counter*);
	void set_damage_receiver(DamageReceiver*);
	void set_effects(Effects*);
	void set_bonus_slot(BonusSlot*);
	void set_left_module(Module*);
	void set_right_module(Module*);
};


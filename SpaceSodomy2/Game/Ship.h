#pragma once
#include <box2d/box2d.h>
#include "Engine.h"
#include "Player.h"
#include "Gun.h"
#include "Counter.h"
#include "Damage_Receiver.h"
#include "iId.h"

class Ship : public iId{
private:
	// Entities
	Player* player = nullptr;
	Engine* engine = nullptr;
	b2Body* body = nullptr;
	Gun* gun = nullptr;
	Counter* hp = nullptr;
	Damage_Receiver* damage_receiver = nullptr;

public:
	Ship();
	~Ship();

	// Get methods
	Player* get_player();
	Engine* get_engine();
	b2Body* get_body();
	Gun* get_gun();
	Counter* get_hp();
	Damage_Receiver* get_damage_receiver();

	// Set methods
	void set_player(Player*);
	void set_engine(Engine*);
	void set_body(b2Body*);
	void set_gun(Gun*);
	void set_hp(Counter*);
	void set_damage_receiver(Damage_Receiver*);
};


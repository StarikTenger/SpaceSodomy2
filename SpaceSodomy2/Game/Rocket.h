#pragma once
#include "iId.h"
#include <box2d/box2d.h>
#include "Engine.h"
#include "Player.h"
#include "Counter.h"
#include "Damage_Receiver.h"
#include "iId.h"
#include "Game_Objects.h"

class Rocket_Brain;

class Rocket : public iId {
private:
	// Entities
	Player* player = nullptr;
	Engine* engine = nullptr;
	b2Body* body = nullptr;
	Counter* hp = nullptr;
	Counter* stamina = nullptr;
	Damage_Receiver* damage_receiver = nullptr;
	Rocket_Brain* brain = nullptr;
	float blast_radius = 0;
	float damage = 0;
	float blast_force = 0;

public:
	Rocket() = default;
	~Rocket() = default;

	// Get methods
	Player* get_player();
	Engine* get_engine();
	b2Body* get_body();
	Counter* get_hp();
	Counter* get_stamina();
	Damage_Receiver* get_damage_receiver();
	Rocket_Brain* get_rocket_brain();
	float get_blast_radius();
	float get_damage();
	float get_blast_force();
	

	// Set methods
	void set_player(Player*);
	void set_engine(Engine*);
	void set_body(b2Body*);
	void set_hp(Counter*);
	void set_stamina(Counter*);
	void set_damage_receiver(Damage_Receiver*);
	void set_rocket_brain(Rocket_Brain*);
	void set_blast_radius(float);
	void set_damage(float);
	void set_blast_force(float);
};

struct Rocket_Prototype {
	float force_linear = 1.f;
	float hp = 20.f;
	float stamina = 1000.f;
	float stamina_recovery = 0.f;
	float range = 5.f;
	float bin_search_accuracy = 10.f;
	float radius = 0.2f;
	float mass = 1.f;
	float blast_radius = 2.f;
	float damage = 20.f;
	float blast_force = 20.f;
};

struct Rocket_Def {
	Player* player;
	Rocket_Prototype base;
	b2Vec2 pos;
	b2Vec2 vel;
	float angle_vel;
	float angle;
};
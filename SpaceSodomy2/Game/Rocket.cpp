#include "pch.h"
#include "Rocket.h"
// Get methods
Player* Rocket::get_player() {
	return player;
}

Engine* Rocket::get_engine() {
	return engine;
}

b2Body* Rocket::get_body() {
	return body;
}

Counter* Rocket::get_hp() {
	return hp;
}

Counter* Rocket::get_stamina() {
	return stamina;
}

Damage_Receiver* Rocket::get_damage_receiver() {
	return damage_receiver;
}

Rocket_Brain* Rocket::get_rocket_brain() {
	return brain;
}
float Rocket::get_blast_radius() {
	return blast_radius;
}
float Rocket::get_damage() {
	return damage;
}
float Rocket::get_blast_force() {
	return blast_force;
}


// Set methods
void Rocket::set_player(Player* val) {
	player = val;
}

void Rocket::set_engine(Engine* val) {
	engine = val;
}

void Rocket::set_body(b2Body* val) {
	body = val;
}


void Rocket::set_hp(Counter* val) {
	hp = val;
}

void Rocket::set_stamina(Counter* val) {
	stamina = val;
}

void Rocket::set_damage_receiver(Damage_Receiver* val) {
	damage_receiver = val;
}

void Rocket::set_rocket_brain(Rocket_Brain* val) {
	brain = val;
}
void Rocket::set_blast_radius(float val) {
	blast_radius = val;
}
void Rocket::set_damage(float val) {
	damage = val;
}
void Rocket::set_blast_force(float val) {
	blast_force = val;
}
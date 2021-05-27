#include "pch.h"
#include "Active_Module.h"
#include <iostream>

Active_Module::Active_Module() {}

void Active_Module::set(b2Body* _body, Player* _player) {
	body = _body;
	player = _player;
}

float Active_Module::get_recharge_time() {
	return recharge_time;
}

b2Body* Active_Module::get_body()
{
	return nullptr;
}

Counter* Active_Module::get_recharge_counter() {
	return recharge_counter;
}

Counter* Active_Module::get_stamina_cooldown_delay_counter() {
	return stamina_cooldown_delay_counter;
}

Event_Manager* Active_Module::get_event_manager() {
	return event_manager;
}

Counter* Active_Module::get_stamina() {
	return stamina;
}
Counter* Active_Module::get_energy() {
	return energy;
}
Effects_Prototype* Active_Module::get_effects() {
	return effects_prototype;
}


void Active_Module::set_recharge_time(float val) {
	recharge_time = val;
}

void Active_Module::set_body(b2Body* val) {
	body = val;
}

void Active_Module::set_recharge_counter(Counter* val) {
	recharge_counter = val;
}

void Active_Module::set_bind(int val) {
	bind = val;
}

void Active_Module::set_event_manager(Event_Manager* val) {
	event_manager = val;
}

void Active_Module::set_stamina(Counter* val) {
	stamina = val;
}
void Active_Module::set_energy(Counter* val) {
	energy = val;
}
void Active_Module::set_effects_prototype(Effects_Prototype* val) {
	effects_prototype = val;
}

void Active_Module::set_effects(Effects* val) {
	effects = val;
}

void Active_Module::activate_default_side_effects() {
	recharge_counter->set(recharge_time);
	stamina->modify(-stamina_cost);
	std::cout << energy->get() << "\n";
	energy->modify(-energy_cost);
	std::cout << energy->get() << "\n";

}

void Active_Module::step(float dt) {
	if (player->get_command_module()->get_command(bind) && 
		recharge_counter->get() < 0 && 
		stamina->get() > stamina_cost && 
		energy->get() >= energy_cost) {
        std::cout << "AM activate\n";
		activate();
	}
}
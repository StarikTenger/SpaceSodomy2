#include "pch.h"
#include "ActiveModule.h"
#include <iostream>

ActiveModule::ActiveModule() {}

void ActiveModule::set(b2Body* _body, Player* _player) {
	body = _body;
	player = _player;
}

float ActiveModule::get_recharge_time() {
	return recharge_time;
}

b2Body* ActiveModule::get_body()
{
	return nullptr;
}

Counter* ActiveModule::get_recharge_counter() {
	return recharge_counter;
}

Counter* ActiveModule::get_stamina_cooldown_delay_counter() {
	return stamina_cooldown_delay_counter;
}

EventManager* ActiveModule::get_event_manager() {
	return event_manager;
}

Counter* ActiveModule::get_stamina() {
	return stamina;
}
Counter* ActiveModule::get_energy() {
	return energy;
}
Effects_Prototype* ActiveModule::get_effects() {
	return effects_prototype;
}


void ActiveModule::set_recharge_time(float val) {
	recharge_time = val;
}

void ActiveModule::set_body(b2Body* val) {
	body = val;
}

void ActiveModule::set_recharge_counter(Counter* val) {
	recharge_counter = val;
}

void ActiveModule::set_bind(int val) {
	bind = val;
}

void ActiveModule::set_event_manager(EventManager* val) {
	event_manager = val;
}

void ActiveModule::set_stamina(Counter* val) {
	stamina = val;
}
void ActiveModule::set_energy(Counter* val) {
	energy = val;
}
void ActiveModule::set_effects_prototype(Effects_Prototype* val) {
	effects_prototype = val;
}

void ActiveModule::set_effects(Effects* val) {
	effects = val;
}

void ActiveModule::activate_default_side_effects() {
	recharge_counter->set(recharge_time);
	stamina->modify(-stamina_cost);
	//std::cout << energy->get() << "\n";
	energy->modify(-energy_cost);
	//std::cout << energy->get() << "\n";

}

void ActiveModule::step(float dt) {
	if (player->get_command_module()->get_command(bind) && 
		recharge_counter->get() < 0 && 
		stamina->get() > stamina_cost && 
		energy->get() >= energy_cost) {
        //std::cout << "AM activate\n";
		activate();
	}
}
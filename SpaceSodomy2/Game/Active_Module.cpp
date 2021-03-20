#include "pch.h"
#include "Active_Module.h"
#include <iostream>

Active_Module::Active_Module() {}

void Active_Module::set(b2Body* _body, Player* _player) {
	body = _body;
	player = _player;
}

float Active_Module::get_recharge_time()
{
	return 0.0f;
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

void Active_Module::set_recharge_time(float val) {
	recharge_time = val;
}

void Active_Module::set_stamina_cooldown_delay_time(float val) {
	stamina_cooldown_delay_time = val;
}

void Active_Module::set_stamina_cooldown_speed(float val) {
	stamina_cooldown_speed = val;
}

void Active_Module::set_body(b2Body* val) {
	body = val;
}

void Active_Module::set_recharge_counter(Counter* _counter) {
	recharge_counter = _counter;
}

void Active_Module::set_stamina_cooldown_delay_counter(Counter* _counter) {
	stamina_cooldown_delay_counter = _counter;
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

void Active_Module::step(float dt) {
	if (player->get_command_module()->get_command(bind) && recharge_counter->get() < 0 && stamina->get() > stamina_consumption) { // TODO: Add here energy & stamina check
		std::cout << "AM activate\n";
		activate();
		recharge_counter->set(recharge_time);
		stamina->modify(-stamina_consumption);
		stamina_cooldown_delay_counter->set(stamina_cooldown_delay_time);
	}
	if (stamina_cooldown_delay_counter->get() < 0) {
		if (stamina->get() < max_stamina - b2_epsilon)
			stamina->set_change_vel(stamina_cooldown_speed);
		else {
			stamina->set(max_stamina);
			stamina->set_change_vel(0);
		}
	}
	else {
		stamina->set_change_vel(0);
	}
}
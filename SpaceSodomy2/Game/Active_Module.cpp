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

void Active_Module::set_recharge_time(float val) {
	recharge_time = val;
}

void Active_Module::set_body(b2Body* val) {
	body = val;
}

void Active_Module::set_recharge_counter(Counter* _counter) {
	recharge_counter = _counter;
}

void Active_Module::set_bind(int val) {
	bind = val;
}

void Active_Module::step(float dt) {
	if (player->get_command_module()->get_command(bind) && recharge_counter->get() < 0) { // TODO: Add here energy & stamina check
		std::cout << "AM activate\n";
		activate();
		recharge_counter->set(recharge_time);
	}
}
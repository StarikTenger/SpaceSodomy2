#include "pch.h"
#include "Active_Module.h"
#include <iostream>

Active_Module::Active_Module() {}

void Active_Module::set(b2Body* _body, Player* _player, Command_Module* _command_module) {
	body = _body;
	command_module = _command_module;
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

void Active_Module::set_recharge_time(float val) {
	recharge_time = val;
}

void Active_Module::set_body(b2Body* val) {
	body = val;
}

void Active_Module::set_bind(int val) {
	bind = val;
}

void Active_Module::step(float dt) {
	time_to_recharge -= dt;

	if (command_module->get_command(bind) && time_to_recharge < 0) { // TODO: Add here energy & stamina check
		std::cout << "AM activate\n";
		activate();
		time_to_recharge = recharge_time;
	}
}
#include "pch.h"
#include "Active_Module.h"

Active_Module::Active_Module() {}

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

int Active_Module::activate() {
	if (time_to_recharge > 0)
		return false;
	return true;
}

void Active_Module::step(float dt) {
	time_to_recharge -= dt;
}
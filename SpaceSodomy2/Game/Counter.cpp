#include "pch.h"
#include "Counter.h"
#include <iostream>

Counter::Counter() {
}

Counter::Counter(float _value) {
	value = _value;
}

Counter::Counter(float _value, float _max_value) {
	value = _value;
	max_value = _max_value;
}

float Counter::get() {
	return value;
}
float Counter::get_max() {
	return max_value;
}

void Counter::set(float _value) {
	value = _value;
}

void Counter::set_max(float _max_value) {
	max_value = _max_value;
}

void Counter::set_delay(float _delay) {
	delay = _delay;
}

void Counter::set_change_vel(float val) {
	change_vel = val;
}

void Counter::modify(float delta) {
	value += delta;
	restart_delay();
}

void Counter::restart_delay() {
	current_delay = delay;
}

void Counter::step(float dt) {
	current_delay -= dt;
	if (current_delay < b2_epsilon) {
		if (value < max_value - b2_epsilon) {

			value = value + dt * change_vel;

		}
		else {
			value = max_value;
		}
	}
}

#include "pch.h"
#include "Counter.h"

Counter::Counter() {
}

Counter::Counter(float _value) {
	value = _value;
}

float Counter::get() {
	return value;
}

void Counter::set(float _value) {
	value = _value;
}

void Counter::set_change_vel(float val) {
	change_vel = val;
}

void Counter::modify(float delta) {
	value += delta;
}

void Counter::step(float dt) {
	value += dt * change_vel;
}

#include "pch.h"
#include "Counter.h"

Counter::Counter() {
}

float Counter::get() {
	return value;
}

void Counter::set(float _value) {
	value = _value;
}

void Counter::modify(float delta) {
	value += delta;
}

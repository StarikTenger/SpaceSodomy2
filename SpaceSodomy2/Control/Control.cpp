#include "pch.h"
#include "framework.h"
#include "Control.h"

void iControl::step() {
	outer_step();
	// Check if the time for the next update has come
	int time_current = aux::get_milli_count();
	int time_delta = time_current - time_prev;
	if (time_delta >= delay) {
		time_prev = time_current;
		inner_step(time_delta);
	}
}

int iControl::is_running() {
	return running;
}

void iControl::run() {
	while (is_running()) {
		step();
	}
}

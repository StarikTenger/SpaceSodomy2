#include "pch.h"
#include "framework.h"
#include "Control.h"

int iControl::is_running() {
	return running;
}

void iControl::run() {
	while (is_running()) {
		step();
	}
}

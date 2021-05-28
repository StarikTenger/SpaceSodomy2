#include "pch.h"
#include "Rocket_Manager.h"
#include <iostream>

Rocket_Manager::Rocket_Manager() {
}

void Rocket_Manager::create_rocket(Rocket_Def def) {
	rockets_to_create.push_back(def);
}

int Rocket_Manager::get_next(Rocket_Def& def) {
	if (!rockets_to_create.size())
		return false;
	def = rockets_to_create.front();
	rockets_to_create.pop_front();
	return true;
}
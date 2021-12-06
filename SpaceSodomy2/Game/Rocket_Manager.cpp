#include "pch.h"
#include "RocketManager.h"
#include <iostream>

RocketManager::RocketManager() {
}

void RocketManager::create_rocket(Rocket_Def def) {
	rockets_to_create.push_back(def);
}

int RocketManager::get_next(Rocket_Def& def) {
	if (!rockets_to_create.size())
		return false;
	def = rockets_to_create.front();
	rockets_to_create.pop_front();
	return true;
}
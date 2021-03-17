#pragma once
#include <deque>
#include "Sound_Def.h"

class Sound_Manager {
private:
	std::deque<Sound_Def> sounds_to_create;

public:
	// Constructor
	Sound_Manager();

	// Create projectile
	void create_sound(Sound_Def);

	// Get another projectile
	int get_next(Sound_Def&);
};


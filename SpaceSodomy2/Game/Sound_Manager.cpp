#include "pch.h"
#include "Sound_Manager.h"

Sound_Manager::Sound_Manager() {
	
}

void Sound_Manager::create_sound(Sound_Def sound_def) {
	sounds_to_create.push_back(sound_def);
}

int Sound_Manager::get_next(Sound_Def& sound_def) {
	if (!sounds_to_create.size())
		return false;
	sound_def = sounds_to_create.front();
	sounds_to_create.pop_front();
	return true;
}

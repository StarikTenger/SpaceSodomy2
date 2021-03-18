#include "pch.h"
#include "Event_Manager.h"

Event_Manager::Event_Manager() {
	
}

void Event_Manager::create_event(Event_Def sound_def) {
	events_to_create.push_back(sound_def);
}

int Event_Manager::get_next(Event_Def& sound_def) {
	if (!events_to_create.size())
		return false;
	sound_def = events_to_create.front();
	events_to_create.pop_front();
	return true;
}

#include "pch.h"
#include "Event_Manager.h"

Event_Manager::Event_Manager() {
	
}

void Event_Manager::create_event(Event_Def event_def) {
	events_to_create.push_back(event_def);
}

int Event_Manager::get_next(Event_Def& event_def) {
	if (!events_to_create.size())
		return false;
	event_def = events_to_create.front();
	events_to_create.pop_front();
	return true;
}

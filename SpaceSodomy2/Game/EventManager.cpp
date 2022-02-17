#include "pch.h"
#include "EventManager.h"

EventManager::EventManager() {
	
}

void EventManager::create_event(EventDef event_def) {
	events_to_create.push_back(event_def);
}

int EventManager::get_next(EventDef& event_def) {
	if (!events_to_create.size())
		return false;
	event_def = events_to_create.front();
	events_to_create.pop_front();
	return true;
}

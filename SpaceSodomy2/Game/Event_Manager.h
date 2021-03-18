#pragma once
#include <string>
#include <deque>
#include <box2d/box2d.h>

struct Event_Def {
	std::string name = "_";
	b2Body* body = nullptr;
};

/// <summary>
/// Event is an sign of anything happened
/// Events are passed to the client via encode
/// Are usually used for sounds and animations on client
/// </summary>
class Event_Manager {
private:
	std::deque<Event_Def> events_to_create;

public:
	// Constructor
	Event_Manager();

	// Create projectile
	void create_event(Event_Def);

	// Get another projectile
	int get_next(Event_Def&);
};


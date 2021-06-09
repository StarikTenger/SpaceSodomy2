#pragma once
#include <string>
#include <deque>
#include <box2d/box2d.h>

struct Event_Def {
	int type = 0;
	b2Body* body = nullptr;
	b2Vec2 pos;
	Event_Def() {}
	Event_Def(int _type, b2Body* _body, b2Vec2 _pos = {}) {
		type = _type;
		body = _body;
		pos = _pos;
		if (body)
			pos = body->GetPosition();
	}
};

/// <summary>
/// Event is an sign of anything happened
/// Events are passed to the client via encode
/// Are usually used for events and animations on client
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


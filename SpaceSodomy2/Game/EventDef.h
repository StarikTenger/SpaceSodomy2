#pragma once
#include <string>
#include <deque>
#include <box2d/box2d.h>

struct EventDef {
	int type = 0;
	b2Body* body = nullptr;
	b2Vec2 pos;
	EventDef() {}
	EventDef(int _type, b2Body* _body, b2Vec2 _pos = {}) {
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
class EventManager {
private:
	std::deque<EventDef> events_to_create;

public:
	// Constructor
	EventManager();

	// Create projectile
	void create_event(EventDef);

	// Get another projectile
	int get_next(EventDef&);
};


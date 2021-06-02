#pragma once
#include <string>
#include <box2d/box2d.h>
#include "iId.h"
#include "Counter.h"

class Event : public iId {
private:
	int type = NONE;
	Counter* playing_offset = nullptr;
	b2Body* body = nullptr;
	b2Vec2 pos;
public:
	enum Types {
		NONE,
		SHOT,
		LASER,
		WALL_HIT,
		FORCE_ACTIVATION,
		BLINK
	};

	Event();

	// Get functions
	int get_type();
	Counter* get_playing_offset();
	b2Vec2 get_pos();

	// Set functions
	void set_type(int);
	void set_playing_offset(Counter*);
	void set_body(b2Body*);
	void set_pos(b2Vec2);

	// Is alive?
	bool is_alive();

};


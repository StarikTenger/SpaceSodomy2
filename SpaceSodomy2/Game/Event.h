#pragma once
#include <string>
#include <map>
#include <vector>
#include <box2d/box2d.h>
#include "iId.h"
#include "Counter.h"

class Event : public iId {
private:
	int type = NONE;
	Counter* playing_offset = nullptr;
	b2Body* body = nullptr;
	b2Vec2 pos;
	std::vector<short> parameters;
public:
	enum Types {
		NONE,
		SHOT,
		LASER,
		DEATH,
		WALL_HIT,
		BONUS_PICKUP,
		MODULE_SHOTGUN,
		MODULE_FORCE,
		MODULE_ROCKET,
		MODULE_DASH,
		MODULE_BLINK
	};

	static int get_parameters_number(int type);

	Event();

	// Get functions
	int get_type();
	Counter* get_playing_offset();
	b2Vec2 get_pos();
	std::vector<short> get_parameters();

	// Set functions
	void set_type(int);
	void set_playing_offset(Counter*);
	void set_body(b2Body*);
	void set_pos(b2Vec2);
	void set_parameters(std::vector<short>);

	// Is alive?
	bool is_alive();
};


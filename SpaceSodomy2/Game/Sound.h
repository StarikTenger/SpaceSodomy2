#pragma once
#include <string>
#include <box2d/box2d.h>
#include "iId.h"
#include "Counter.h"

class Sound : public iId {
private:
	std::string name = "";
	Counter* playing_offset = nullptr;
	b2Body* body = nullptr;
	b2Vec2 pos;
public:
	Sound();

	// Get functions
	std::string get_name();
	Counter* get_playing_offset();
	b2Vec2 get_pos();

	// Set functions
	void set_name(std::string);
	void set_playing_offset(Counter*);
	void set_body(b2Body*);
	void set_pos(b2Vec2);

	// Is alive?
	bool is_alive();

};


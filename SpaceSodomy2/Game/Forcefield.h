#pragma once
#include "iId.h"
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>

class Forcefield : public iId {
private:
    b2Vec2 force;
    std::vector<b2Vec2> vertices;
    b2Body* body = nullptr;

	b2Vec2 origin_pos;
	b2Vec2 box_size;

public:
	// Constructor
	Forcefield() = default;
	// Get mehods
	std::vector<b2Vec2> get_vertices();
	b2Body* get_body();
	b2Vec2 get_force();
	// Creating a forcefield
	void set(b2World*, std::vector<b2Vec2>, b2Vec2 force);
	bool apply(b2Body*, float dt);

	b2Vec2 get_origin_pos();
	b2Vec2 get_box_size();

	struct Point {
		b2Vec2 force;
		b2Vec2 pos;
	};
	std::vector<Forcefield::Point> get_pts_for_grid(float step);
};


#pragma once
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <vector>
#include "iId.h"

// A polygon wall
class Wall : public iId{
private:
	int orientation = OUTER;
	std::vector<b2Vec2> vertices;
	b2Body* body = nullptr;
	b2Vec2 origin_pos;
	b2Vec2 box_size;

	//should be odd
	int outer_bound_of_inner_wall_textures = 3;
public:

	enum Orientation {
		INNER, OUTER
	};
	// Constructor
	Wall();
	// Get mehods
	std::vector<b2Vec2> get_vertices();
	b2Body* get_body();
	int get_orientation();
	// Creating a wall with the  
	void set(b2World*, std::vector<b2Vec2>, int _orientation = OUTER);
	//void set_orientation(int);

	//A invariant unique id for walls
	int hash();

	// Initializes params for drawing
	void init_draw();

	b2Vec2 get_origin_pos();
	b2Vec2 get_box_size();
	int get_outer_bound_of_inner_wall_textures();
};


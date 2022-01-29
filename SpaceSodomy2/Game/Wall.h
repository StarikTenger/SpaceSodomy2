#pragma once
#include <box2d/box2d.h>
#include <./../AuxLib/AuxLib.h>
#include <vector>
#include "iId.h"

// A polygon wall
class Wall : public iId{
private:
	int orientation = OUTER;
	int type = STANDART;
	std::vector<b2Vec2> vertices;
	b2Body* body = nullptr;

	b2Vec2 origin_pos;
	b2Vec2 box_size;
public:
	enum Type {
		STANDART,
		SPIKED,
		GHOST
	};
	enum Orientation {
		INNER, OUTER
	};
	// Constructor
	Wall();
	// Get mehods
	std::vector<b2Vec2> get_vertices();
	b2Body* get_body();
	int get_orientation();
	int get_type();
	// Creating a wall  
	void set(b2World*, std::vector<b2Vec2>, int _orientation = OUTER, int _type = STANDART);
	//void set_orientation(int);


	// Initializes params for drawing
	void init_drawing(float wall_width);

	b2Vec2 get_origin_pos();
	b2Vec2 get_box_size();
};


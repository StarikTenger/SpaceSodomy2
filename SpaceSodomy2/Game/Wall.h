#pragma once
#include <box2d/box2d.h>
#include <vector>

// A polygon wall
class Wall {
private:
	std::vector<b2Vec2> vertices;
	b2Body* body = nullptr;
public:
	// Constructor
	Wall();
	// Get mehods
	std::vector<b2Vec2> get_vertices();
	b2Body* get_body();
	// Creating a wall with the body
	void set(b2World*, std::vector<b2Vec2>);
};


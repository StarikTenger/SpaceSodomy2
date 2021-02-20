#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Camera {
private:
	b2Vec2 pos = b2Vec2_zero;
	b2Vec2 borders = b2Vec2_zero;
	float angle = 0;
	float scale = 1;
public:
	// Constructors
	Camera();
	// Get methods
	b2Vec2 get_pos();
	b2Vec2 get_borders();
	float get_angle();
	float get_scale();
	// Set methods
	void set_pos(b2Vec2 val);
	void set_borders(b2Vec2 val);
	void set_angle(float val);
	void set_scale(float val);
	// Apply camera to scene
	void apply(sf::RenderWindow* window);
};


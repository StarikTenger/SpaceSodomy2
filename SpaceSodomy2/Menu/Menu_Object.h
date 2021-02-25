#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <Draw/Draw.h>
#include <AuxLib/AuxLib.h>

class Menu_Object
{
private:
	std::string texture_name;
	sf::Color color;
	Draw* draw;
	b2Vec2 pos, scale;
	bool active = 0;
public:
	Menu_Object();
	Menu_Object(std::string texture_name_, Draw* draw_, b2Vec2 pos_, b2Vec2 scale_);

	// Get methods
	std::string get_texture_name();
	Draw* get_draw();
	b2Vec2 get_pos();
	b2Vec2 get_scale();
	bool get_active();

	// Set methods
	void set_texture_name(std::string texture_name_);
	void set_draw(Draw* draw_);
	void set_pos(b2Vec2 pos_);
	void set_scale(b2Vec2 scale_);

	void primitive_step();
};


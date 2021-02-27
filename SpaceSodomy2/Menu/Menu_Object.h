#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <Draw/Draw.h>
#include <AuxLib/AuxLib.h>

class Menu_Object
{
private:
	int id;
	std::string texture_name;
	sf::Color color;
	Draw* draw;
	b2Vec2 pos, scale;
	bool active = 0;
public:
	Menu_Object();
	Menu_Object(int id_, std::string texture_name_, Draw* draw_, b2Vec2 pos_, b2Vec2 scale_, sf::Color color_);

	// Get methods
	int get_id();
	std::string get_texture_name();
	Draw* get_draw();
	b2Vec2 get_pos();
	b2Vec2 get_scale();
	sf::Color get_color();
	bool get_active();

	// Set methods
	void set_id(int id_);
	void set_texture_name(std::string texture_name_);
	void set_draw(Draw* draw_);
	void set_pos(b2Vec2 pos_);
	void set_scale(b2Vec2 scale_);
	void set_color(sf::Color color_);

	void primitive_step();
};


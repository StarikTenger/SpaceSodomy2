#pragma once
#include "Menu_Object.h"
class Slider : public Menu_Object {
private:
	sf::RectangleShape slider;
	sf::RectangleShape axis;
	sf::Font font;
	sf::Text text;
	int min_value;
	int* max_value;
	b2Vec2 cord; // slider pointer pos
	b2Vec2 axis_scale;
	b2Vec2 slider_scale;
	b2Vec2 real_axis_scale;
	b2Vec2 real_slider_scale;
	float slider_value;
	bool font_active = 0; // is font active
	bool slider_active = 0; // is slider active
	sf::Vector2f mouse_pos_;
	sf::Text returnText(b2Vec2 pos_, std::string z, int font_size); // return text object with params
	void logic(sf::RenderWindow* window); // work with mouse
	void draw(sf::RenderWindow* window); // rendering slider
public:
	Slider();
	void init();

	// Set methods
	void create(int min, int* max); // set min max vals
	void set_slider_value(float value_); // set val
	void set_slider_percent_value(float percent_value_); // set percent val
	void set_axis_scale(b2Vec2 axis_scale_);
	void set_slider_scale(b2Vec2 slider_scale_);

	// Get methods
	float get_slider_value();
	b2Vec2 get_axis_scale();
	b2Vec2 get_slider_scale();

	void step();
};


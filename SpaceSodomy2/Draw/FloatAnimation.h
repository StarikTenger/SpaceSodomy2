#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class FloatAnimation {
public:
	struct State {
		std::string image = "";
		b2Vec2 pos;
		b2Vec2 scale;
		sf::Color color = sf::Color(255, 255, 255);
		float angle = 0;
		State operator+(const State& x);
		State operator*(const float x);
	};

private:
	// Animations on each layer are drawn separately
	int layer = 0;
	// state_current continiously changes from state_begin to state_end
	State state_begin; 
	State state_end;
	State state_current;
	// time_current changes from 0 to time_max
	float time_current = 0;
	float time_max = 0;

public:
	FloatAnimation();
	FloatAnimation(std::string _image, State _state_begin, State _state_end, float _time, int layer = 0);

	// Getters
	State get_state_current();
	int get_layer();

	// Animation is alive until it time reaches time_max
	bool is_alive();

	void step(float dt);
};


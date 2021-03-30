#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Float_Animation {
public:
	struct State {
		std::string image = "";
		b2Vec2 pos;
		b2Vec2 scale;
		sf::Color color;
		float angle = 0;
		State operator+(const State& x);
		State operator*(const float x);
	};

private:
	State state_begin; 
	State state_end;
	State state_current;
	float time_current = 0;
	float time_max = 0;

public:
	Float_Animation();
	Float_Animation(std::string _image, State _state_begin, State _state_end, float _time);

	State get_state_current();
	bool is_alive();

	void step(float dt);
};


#include "pch.h"
#include "FloatAnimation.h"

FloatAnimation::State FloatAnimation::State::operator+(const State& x) {
	FloatAnimation::State state;
	state.pos = pos + x.pos;
	state.scale = scale + x.scale;
	state.color = color + x.color;
	state.angle = angle + x.angle;
	return state;
}

FloatAnimation::State FloatAnimation::State::operator*(const float x) {
	FloatAnimation::State state;
	state.pos = x * pos;
	state.scale = x * scale;
	state.color = { 
		(sf::Uint8)((int)color.r * x),
		(sf::Uint8)((int)color.g * x),
		(sf::Uint8)((int)color.b * x),
		(sf::Uint8)((int)color.a * x) };
	state.angle = angle * x;
	return state;
}

FloatAnimation::FloatAnimation() {}

FloatAnimation::FloatAnimation(std::string _image, State _state_begin, State _state_end, float _time, int _layer) {
	layer = _layer;
	state_current = state_begin = _state_begin;
	state_begin.image = _image;
	state_end = _state_end;
	time_max = _time;
}

FloatAnimation::State FloatAnimation::get_state_current() {
	state_current.image = state_begin.image;
	return state_current;
}

int FloatAnimation::get_layer() {
	return layer;
}

bool FloatAnimation::is_alive() {
	return time_current < time_max;
}

void FloatAnimation::step(float dt) {
	time_current += dt;
	float progress = time_current / time_max;
	state_current = state_begin * (1 - progress) + state_end * progress;
}



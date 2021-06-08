#include "pch.h"
#include "Event.h"

Event::Event() {
}

int Event::get_type() {
	return type;
}

Counter* Event::get_playing_offset() {
	return playing_offset;
}

b2Vec2 Event::get_pos() {
	if (body)
		return body->GetPosition();
	return pos;
}

void Event::set_type(int val) {
	type = val;
}

void Event::set_playing_offset(Counter* val) {
	playing_offset = val;
}

void Event::set_body(b2Body* val) {
	body = val;
}

void Event::set_pos(b2Vec2 val) {
	pos = val;
}

bool Event::is_alive() {
	// TODO: Events lasts for one second (temporary solution)
	return playing_offset->get() < 1;
}

#include "pch.h"
#include "Sound.h"

Sound::Sound() {
}

std::string Sound::get_name() {
	return name;
}

Counter* Sound::get_playing_offset() {
	return playing_offset;
}

b2Body* Sound::get_body() {	
	return body;
}

void Sound::set_name(std::string val) {
	name = val;
}

void Sound::set_playing_offset(Counter* val) {
	playing_offset = val;
}

void Sound::set_body(b2Body* val) {
	body = val;
}

bool Sound::is_alive() {
	// Sounds lasts for one second (temporary solution)
	return playing_offset->get() < 1;
}

// Audio.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "Audio.h"

Audio::Audio() {}

void Audio::load_sound(std::string name, std::string path_to_sound) {
	sf::SoundBuffer* buffer = new sf::SoundBuffer();
	buffer->loadFromFile(path_to_sound);
	sf::Sound* sound = new sf::Sound();
	sound->setBuffer(*buffer);
	sounds[name] = sound;
}

void Audio::set_draw(Draw* draw_) {
	draw = draw_;
}

Draw* Audio::get_draw() {
	return draw;
}

void Audio::load_sounds(std::string path) {
	std::cout << "Start loading\n";
	std::ifstream file(path);
	while (file) {
		std::string name, path;
		file >> name >> path;
		std::cout << "loaded: " << name << " " << path << "\n";
		load_sound(name, path);
	}
	std::cout << "Finish loading\n";
}

void Audio::play(int id, std::string name, b2Vec2 pos, double z, double volume) {
	while (-sound_timeouts.top().first < aux::get_milli_count()) {
		activeSounds.erase(sound_timeouts.top().second);
		sound_timeouts.pop();
	}
	sf::Listener::setDirection(1.f, 0.f, 0.f);
	sf::Sound *sound = new sf::Sound;
	*sound = *sounds[name];
	sound->setPosition(z, pos.x, pos.y);
	sound->setVolume(volume);
	sound->play();
	activeSounds[id] = sound;
	sound_timeouts.push({ -aux::get_milli_count() - sound->getBuffer()->getDuration().asMilliseconds(), id });
}

void Audio::play(int id, std::string name, b2Vec2 pos, double volume) {
	pos -= draw->get_camera()->get_pos();
	pos = aux::rotate(draw->get_camera()->get_pos(), pos, draw->get_camera()->get_angle());
	play(id, name, pos, -5, volume);
}

void Audio::update_sound(int id, std::string name, b2Vec2 pos) {
	if (activeSounds.count(id)) {
		activeSounds[id]->setPosition(sf::Vector3f(pos.x, pos.y, -5));
	}
	else {
		play(id, name, pos, 100);
	}
}
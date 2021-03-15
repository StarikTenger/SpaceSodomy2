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

void Audio::play(std::string name, b2Vec2 pos, double z, double volume, sf::Sound* sound) {
	for (int i = 0; i < activeSounds.size(); i++) {
		if (activeSounds[i]->getStatus() != sf::Sound::Playing) {
			//std::cout << activeSounds.size() << " deleted\n";
			delete activeSounds[i];
			activeSounds.erase(activeSounds.begin() + i);
			i--;
		}
	}
	sf::Listener::setDirection(1.f, 0.f, 0.f);
	*sound = *sounds[name];
	sound->setPosition(z, pos.x, pos.y);
	sound->setVolume(volume);
	sound->play();
	activeSounds.push_back(sound);
}

void Audio::play(std::string name, b2Vec2 pos, double volume, sf::Sound* sound) {
	pos -= draw->get_camera()->get_pos();
	pos = aux::rotate(draw->get_camera()->get_pos(), pos, draw->get_camera()->get_angle());
	play(name, pos, -5, volume, sound);
}
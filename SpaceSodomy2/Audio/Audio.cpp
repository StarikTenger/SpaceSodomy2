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

void Audio::load_music(std::string name, std::string path_to_music) {
	sf::Music* music = new sf::Music();
	if (!music->openFromFile(path_to_music)) {
		std::cout << "Can't load soundtrack " << name << "\n";
		return; // error
	}
	soundtrack[name] = music;
}

void Audio::set_draw(Draw* draw_) {
	draw = draw_;
}
void Audio::set_sound_volume(double volume) {
	sound_volume = volume;
	for (auto sound : activeSounds)
		activeSounds[sound.first]->setVolume(activeSounds_mode[sound.first] * volume);
}
void Audio::set_music_volume(double volume) {
	music_volume = volume;
	for (auto music : soundtrack)
		soundtrack[music.first]->setVolume(soundtrack_mode[music.first] * volume);
}


Draw* Audio::get_draw() {
	return draw;
}
double Audio::get_sound_volume() {
	return sound_volume;
}
double Audio::get_music_volume() {
	return music_volume;
}

void Audio::load_sounds(std::string path) {
	std::cout << "Start loading\n";
	std::ifstream file(path);
	std::stringstream loadedFile = aux::comment(file);
	while (loadedFile) {
		std::string name, path;
		loadedFile >> name >> path;
		std::cout << "loaded: " << name << " " << path << "\n";
		load_sound(name, path);
	}
	std::cout << "Finish loading\n";
}

void Audio::load_soundtrack(std::string path) {
	std::cout << "Musics start loading\n";
	std::ifstream file(path);
	std::stringstream loadedFile = aux::comment(file);
	while (loadedFile) {
		std::string name, path;
		loadedFile >> name >> path;
		std::cout << "loaded: " << name << " " << path << "\n";
		load_music(name, path);
	}
	std::cout << "Finish loading\n";
}

void Audio::play(int id, std::string name, b2Vec2 pos, double z, double volume, bool looped) {
	while (!sound_timeouts.empty() && -sound_timeouts.top().first < aux::get_milli_count()) {
		activeSounds[sound_timeouts.top().second]->stop();
		delete activeSounds[sound_timeouts.top().second];
		activeSounds.erase(sound_timeouts.top().second);
   		sound_timeouts.pop();
	}
	sf::Listener::setDirection(1.f, 0.f, 0.f);
	sf::Listener::setPosition(0.f, 0.f, 0.f);
	sf::Sound* sound = new sf::Sound();
	if (!sounds.count(name))
		return;
	*sound = *sounds[name];
	sound->setPosition(pos.x, pos.y, z);
	sound->setVolume(volume);
	sound->setRelativeToListener(1);
	sound->setLoop(looped);
	activeSounds[id] = sound;
	sound->play();
	if (!looped)
		sound_timeouts.push({ -aux::get_milli_count() - sound->getBuffer()->getDuration().asMilliseconds(), id });
	//std::cout << sound_timeouts.size() << sound_timeouts.top().first << " " << sound_timeouts.top().second << "\n";
}

void Audio::play(int id, std::string name, b2Vec2 pos, double volume, bool looped) {
	play(id, name, pos, -5, volume, looped);
}

void Audio::update_sound(int id, std::string name, b2Vec2 pos, double volume_mode, bool looped) {
	activeSounds_mode[id] = volume_mode;
	pos -= draw->get_camera()->get_pos();
	pos = aux::rotate(pos, -draw->get_camera()->get_angle() - b2_pi / 2);
	if (activeSounds.count(id)) {
		activeSounds[id]->setPosition(sf::Vector3f(pos.x, pos.y, -5));
		activeSounds[id]->setVolume(sound_volume * activeSounds_mode[id]);
	}
	else {
		if (!sounds.count(name))
			return;
		//std::cout << "NEW SOUND " << id << " " << name << "\n";
		play(id, name, pos, sound_volume * activeSounds_mode[id], looped);
	}
	activeSounds[id]->setLoop(looped);
}

void Audio::update_music(std::string name, double volume_mode) {
	soundtrack[name] = soundtrack[name];
	soundtrack[name]->setLoop(1);
	soundtrack_mode[name] = volume_mode;
	soundtrack[name]->setVolume(music_volume * volume_mode);
}

void Audio::start_music(std::string name) {
	soundtrack[name]->play();
}
void Audio::stop_music(std::string name) {
	soundtrack[name]->stop();
}

std::string Audio::get_music_by_number(int val) {
	if (soundtrack.size() == 0) {
		std::cout << "no music found\n";
		return "";
	}
	val %= soundtrack.size();
	auto it = soundtrack.begin();
	for (int i = 0; i < val; i++) {
		it++;
	}
	return it->first;
}
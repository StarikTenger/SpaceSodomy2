#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <Draw/Draw.h>
#include <string>
#include <map>
#include <queue>
#include <fstream>

class Audio {
private:
	std::map<std::string, sf::Sound*> sounds;
	std::map<std::string, sf::Music*> musics;
	std::map<int, sf::Sound*> activeSounds;
	std::priority_queue<std::pair<int, int>> sound_timeouts;
	void load_sound(std::string name, std::string path_to_sound);
	void load_music(std::string name, std::string path_to_music);
	Draw* draw;
public:
	Audio();

	void set_draw(Draw* draw_);
	
	Draw* get_draw();

	void load_sounds(std::string path = "sounds.conf");
	void load_musics(std::string path = "musics.conf");

	void play(int id, std::string name, b2Vec2 pos, double z, double volume);
	void play(int id, std::string name, b2Vec2 pos, double volume);

	void update_sound(int id, std::string name, b2Vec2 pos, double volume);
	void update_music(std::string name, double volume);
	void start_music(std::string name);
	void stop_music(std::string name);

	std::string get_music_by_number(int val);
};
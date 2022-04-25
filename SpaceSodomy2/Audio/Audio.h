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
	std::map<std::string, sf::Music*> soundtrack;
	std::map<int, sf::Sound*> activeSounds;
	std::map<int, double> activeSounds_mode;
	std::map<std::string, double> soundtrack_mode;
	std::priority_queue<std::pair<int, int>> sound_timeouts;
	void load_sound(std::string name, std::string path_to_sound);
	void load_music(std::string name, std::string path_to_music);
	Draw* draw;
	// Sound & music volume
	int sound_volume = 100, music_volume = 100;
public:
	Audio();

	void set_draw(Draw* draw_);
	void set_sound_volume(double volume);
	void set_music_volume(double volume);
	
	Draw* get_draw();
	double get_sound_volume();
	double get_music_volume();

	void load_sounds(std::string path = "sounds.conf");
	void load_soundtrack(std::string path = "soundtrack.conf");

	void play(int id, std::string name, b2Vec2 pos, double z, double volume, bool looped);
	void play(int id, std::string name, b2Vec2 pos, double volume, bool looped);

	void update_sound(int id, std::string name, b2Vec2 pos, double volume_mode, bool looped);
	void update_music(std::string name, double volume_mode);
	void start_music(std::string name);
	void stop_music(std::string name);

	std::string get_music_by_number(int val);
};
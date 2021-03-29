#pragma once
#include <Game/Game.h>
#include <Draw/Draw.h>
#include <Audio/Audio.h>
#include <Menu/Bar.h>
#include <Menu/Constant_Text.h>

class Game_Client : public Game {
private:
	Draw* draw = nullptr;
	Audio* audio = nullptr;
	std::string gun_name = "default";
	std::string hull_name = "default";
public:
	// Set methods
	void set_draw(Draw* _draw);
	void set_audio(Audio* _audio);
	
	// Get methods
	Draw* get_draw();
	Audio* get_audio();
	std::string get_gun_name();
	std::string get_hull_name();

	// Set methods
	void set_gun_name(std::string val);

	void display(int id);

	// Decodes class from string
	void decode(std::string source);

	// Get ship
	Ship* get_ship(int id);

	// Get players
	std::map<int, Player*>* get_players();

	int* sound_volume = nullptr;

	// Load setup
	void load_setup(std::string path);
	void save_setup(std::string path);
};
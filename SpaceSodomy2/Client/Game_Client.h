#pragma once
#include <Game/Game.h>
#include <Draw/Draw.h>
#include <Audio/Audio.h>

class Game_Client : public Game {
private:
	Draw* draw = nullptr;
	Audio* audio = nullptr;

public:
	// Set methods
	void set_draw(Draw* _draw);
	void set_audio(Audio* _audio);
	
	// Get methods
	Draw* get_draw();
	Audio* get_audio();

	void display(int id);

	// Decodes class from string
	void decode(std::string source);

	// Get ship
	Ship* get_ship(int id);
};


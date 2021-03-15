#pragma once
#include <Game/Game.h>
#include <Draw/Draw.h>

class Game_Client : public Game {
private:
	Draw* draw = nullptr;
public:
	// Set methods
	void set_draw(Draw* _draw);
	
	// Get methods
	Draw* get_draw();

	void display(int id);

	// Decodes class from string
	void decode(std::string source);
};


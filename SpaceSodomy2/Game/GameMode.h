#pragma once
#include "GameReadable.h"

// This is a simple example of game mode. Currently built for deathmatch
class GameMode {
private:
	int kills_to_win = 2;
	int time_limit = 10 * 60 * 1000; // Ten minutes
	const GameReadable& game;
public:
	GameMode(const GameReadable& game) : game(game) {};

	void step();
	bool is_game_finished();
};


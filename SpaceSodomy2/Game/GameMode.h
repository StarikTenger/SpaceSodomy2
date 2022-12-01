#pragma once
#include "GameReadable.h"
#include <string>

// This is a simple example of game mode. Currently built for deathmatch
class GameMode {
private:
	int kills_to_win = 100;
	int time_limit = 60* 60; // Sixty minutes
	const GameReadable& game;
	std::string message = ""; // Message shown when game is over
public:
	GameMode(const GameReadable& game) : game(game) {};

	void step();
	bool is_game_finished();
	int comp_players(Player* p1, Player* p2); // Determines, which player has better score
	int winner_id();
	int get_time_left(); // Time left in seconds
};


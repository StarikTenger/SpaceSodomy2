#include "pch.h"
#include "GameMode.h"
#include "Player.h"

void GameMode::step() {
}

bool GameMode::is_game_finished() {
	bool game_finished = false;
	for (const auto player_pair : game.players) {
		auto id = player_pair.first;
		auto player = player_pair.second;

		if (player->get_id() != -1 && player->get_kills() >= kills_to_win) {
			message = player->get_name() + " wins";
			game_finished = true;
		}
	}
	if (game.time > time_limit) {
		game_finished = true;
	}
	return game_finished;
}

int GameMode::comp_players(Player* p1, Player* p2) {
	if (p1->get_kills() > p1->get_kills())
		return -1;
	if (p1->get_kills() < p1->get_kills())
		return 1;
	if (p1->get_deaths() < p1->get_deaths())
		return -1;
	if (p1->get_deaths() > p1->get_deaths())
		return 1;
	return 0;
}

int GameMode::winner_id() {
	int best_id = 0;
	int best_score = 0;
	for (const auto player_pair : game.players) {
		auto id = player_pair.first;
		auto player = player_pair.second;

		if (player->get_id() != -1 && player->get_kills() >= kills_to_win) {
			message = player->get_name() + " wins";
			return id;
		}
	}
	return best_id;
}


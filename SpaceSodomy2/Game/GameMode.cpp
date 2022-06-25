#include "pch.h"
#include "GameMode.h"
#include "Player.h"

void GameMode::step() {
}

bool GameMode::is_game_finished() {
	/*for (const auto player_pair : game.players) {
		auto id = player_pair.first;
		auto player = player_pair.second;

		if (player->get_kills() >= kills_to_win) {
			return true;
		}
	}
	if (game.time > time_limit) {
		return true;
	}*/
	return false;
}

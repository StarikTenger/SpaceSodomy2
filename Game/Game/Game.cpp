// Game.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "Game.h"

Game::Game() {}

void Game::step(float dt) {
	physics.Step(dt, 10, 10);
}
#include <iostream>
#include <Game.h>
#include <Client_Network.h>
#include <Server_Network.h>

int main() {
	Game game;
	game.step1(1. / 50);
	//std::cout << game.test().x << " " << game.test().y << "\n";
	std::cout << "End\n";
}
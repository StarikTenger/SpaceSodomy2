#pragma once
#include <Network/Client_Network.h>
#include <AuxLib/AuxLib.h>
#include <Draw/Draw.h>
#include "Game_Client.h"

class Control {
private:
	// Period of drawing & sending in ms
	int delay = 20;
	// Last time of activity
	int time_prev = 0;

	Client_Network network;
	Game_Client game;
	Draw draw;
public:
	Control();
	// Func to call regulary
	void step();
};


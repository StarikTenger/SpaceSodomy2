#pragma once
#include<Menu/Bar.h>
#include<Client/Game_Client.h>
#include<AuxLib/AuxLib.h>
#include<Network/Client_Network.h>

class HUD_Processing {
private:
	Bar HP_bar, stamina_bar;

	Game_Client* game;
	Client_Network* player_network;

	void apply_bar(Bar* bar, std::stringstream* config, Draw* draw, b2Vec2* mouse_pos, aux::Keyboard* keyboard);
public:
	HUD_Processing(Draw* draw, b2Vec2* mouse_pos, aux::Keyboard* keyboard, Game_Client* game_, Client_Network* player_network_);

	void step();
};


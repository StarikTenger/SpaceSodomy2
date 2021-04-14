#pragma once
#include<Menu/Bar.h>
#include<Client/Game_Client.h>
#include<AuxLib/AuxLib.h>
#include<Network/Client_Network.h>

class HUD_Processing {
private:
	Bar HP_bar, stamina_bar;
	Constant_Text time_to_respawn, press_r_to_respawn;

	int table_use_windows_cords;
	b2Vec2 table_pos;
	float table_character_size;
	float table_name_indent;
	b2Vec2 table_indent;

	Game_Client* game;
	Client_Network* player_network;

	Draw* draw;
	b2Vec2* mouse_pos;
	aux::Keyboard* keyboard;

	std::string get_respawn_button_name(std::string path);
	void apply_bar(Bar* bar, std::stringstream* config);
	void table_step();

	int HP_bar_max_val;
	int stamina_bar_max_val;
	float HP_bar_val;
	float stamina_bar_val;
public:
	HUD_Processing(Draw* draw_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_, Game_Client* game_, Client_Network* player_network_);

	void step();
};


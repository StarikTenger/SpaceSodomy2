#pragma once
#include<Client/Game_Client.h>
#include<AuxLib/AuxLib.h>
#include<Network/Client_Network.h>
#include<TGUI/TGUI.hpp>
#include<TGUI/Backend/SFML-Graphics.hpp>

class HUD_Processing {
private:
	Game_Client* game;
	Client_Network* player_network;

	Draw* draw;
	b2Vec2* mouse_pos;
	aux::Keyboard* keyboard;

	std::string bonus_button_name, left_module_button_name, right_module_button_name, respawn_button_name;

	void get_buttons_names(tgui::Gui &gui, std::string path);
	void table_step(tgui::Gui &gui, float scale);

	std::queue<int>* frame_marks;

	std::vector<b2Vec2> get_vertices(float cur_pos, b2Vec2 center, b2Vec2 scale);
public:
	float interface_scale = 3;

	HUD_Processing();
	HUD_Processing(tgui::Gui &gui, Draw* draw_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_, Game_Client* game_,
		Client_Network* player_network_, std::queue<int>* frame_marks_);

	void step(tgui::Gui& gui);
};


#pragma once
#include<Client/GameClient.h>
#include<AuxLib/AuxLib.h>
#include<Network/ClientNetwork.h>
#include<TGUI/TGUI.hpp>
#include<TGUI/Backend/SFML-Graphics.hpp>

class HUDProcessing {
private:
	GameClient* game;
	ClientNetwork* player_network;

	Draw* draw;
	b2Vec2* mouse_pos;
	aux::Keyboard* keyboard;

	std::string bonus_button_name, left_module_button_name, right_module_button_name, respawn_button_name;

	void get_buttons_names(tgui::Gui &gui, std::string path);
	void table_step(tgui::Gui &gui, float scale);

	std::queue<int>* frame_marks;

	std::vector<b2Vec2> get_vertices(float cur_pos, b2Vec2 center, b2Vec2 scale);

	bool reload_icons = true;
	std::string bonus_icon_name = "";
public:
	float interface_scale = 3;

	HUDProcessing();
	HUDProcessing(tgui::Gui &gui, Draw* draw_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_, GameClient* game_,
		ClientNetwork* player_network_, std::queue<int>* frame_marks_);

	void step(tgui::Gui& gui);
};


#pragma once
#include <AuxLib/AuxLib.h>
#include <Client/GameClient.h>
#include <string>
#include <Client/Replay.h>
#include <Network/ClientNetwork.h>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "KeybindingBox.h"

class MenuProcessing {
private:
	aux::Keyboard* keyboard;
	b2Vec2* mouse_pos;
	int current_id = 1;
	ClientNetwork* network;
	std::map<int, std::pair<int, int>> id_to_keyit;
	void init_tgui();
	void save_config(std::string path, std::string address_, int port_, int id_, std::string name_);
	void load_config(std::string path, std::string* address_, std::string* port_,
		std::string* id_, std::string* name_);
	void load_keys(std::string path, std::vector<std::vector<std::string*>>* keys);
	void load_HUD_settings(std::string path);
	void save_HUD_settings(std::string path);
	void load_sound(std::string path);
	void save_sound(std::string path);
	void init_multiplayer_menu(std::string file_name);
	void group_formating(std::string groupname, float aspectratio, float textscale);
	void close_widgets(tgui::Container::Ptr container);
	void close_groups();
	Draw* draw;
	GameClient* game;
	Replay* replay;
	tgui::Gui* _gui;
	bool disactivated = 0;
	bool* reload;
	int module_num = 1;
	bool slider_changing = 0;
public:
	MenuProcessing();
	bool active = 1;
	void toggle_active();
	int text_field_active = 0;
	std::vector<std::vector<std::string*>> keys_menu_vec;
	void save_keys(std::string path, std::vector<std::vector<std::string*>> keys);
	void init(tgui::Gui& gui, Draw* draw, b2Vec2* mouse_pos_,
		aux::Keyboard* keyboard_, ClientNetwork* network_,
		GameClient* game_, Replay* replay_,
		bool* reload_);
	void step();
};



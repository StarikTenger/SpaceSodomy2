#pragma once
#include<AuxLib/AuxLib.h>
#include<Client/Game_Client.h>
#include<string>
#include<Client/Replay.h>
#include<Network/Client_Network.h>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "KeybindingBox.h"

class Menu_Processing {
private:
	aux::Keyboard* keyboard;
	b2Vec2* mouse_pos;
	int current_id = 1;
	Client_Network* network;
	std::queue<int> events; // menu events
	std::map<std::string, int> name_to_id;
	std::map<int, std::pair<int, int>> id_to_keyit;
	void init_tgui(tgui::Gui& gui);
	void save_config(std::string path, std::string address_, int port_, int id_, std::string name_);
	void load_config(std::string path, std::string* address_, std::string* port_,
		std::string* id_, std::string* name_, tgui::Gui &gui);
	void load_keys(std::string path, std::vector<std::vector<std::string*>>* keys, tgui::Gui& gui);
	void load_HUD_settings(std::string path, tgui::Gui &gui);
	void save_HUD_settings(std::string path);
	void load_sound(std::string path, tgui::Gui &gui);
	void save_sound(std::string path);
	void init_multiplayer_menu(std::string file_name, tgui::Gui& gui);
	Draw* draw;
	Game_Client* game;
	Replay* replay;
	tgui::Gui* _gui;
	bool disactivated = 0;
	bool shader_active = 1;
	bool* reload;
	int module_num = 1;
public:
	Menu_Processing();
	bool active = 1;
	int text_field_active = 0;
	std::vector<std::vector<std::string*>> keys_menu_vec;
	void save_keys(std::string path, std::vector<std::vector<std::string*>> keys);
	void init(tgui::Gui& gui, Draw* draw, b2Vec2* mouse_pos_,
		aux::Keyboard* keyboard_, Client_Network* network_,
		Game_Client* game_, Replay* replay_,
		bool* reload_);
	void step();
};



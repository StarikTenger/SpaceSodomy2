#pragma once
#include<Menu/Menu.h>
#include<AuxLib/AuxLib.h>
#include<string>

class Menu_Processing {
private:
	Menu main_menu; // main menu object
	Menu config_menu; // config menu object
	Menu keys_menu; // keys menu object
	Menu settings_menu; // settings menu object
	std::vector<Menu*> menus;
	aux::Keyboard* keyboard;
	b2Vec2* mouse_pos;
	int current_id = 1;
	bool* reload;
	std::queue<int> events; // menu events
	std::map<int, std::string> text_fields_strings; // texts from text fields
	std::map<int, int> sliders_vals; // vals from sliders
	std::map<std::string, int> name_to_id;
	std::map<int, std::string> id_to_name;
	std::vector<std::vector<std::string*>> keys_menu_vec;
	void save_config(std::string path, std::string address_, int port_, int id_, std::string name_);
	void load_config(std::string path, std::string* address_, std::string* port_,
		std::string* id_, std::string* name_);
	void save_keys(std::string path, std::vector<std::vector<std::string*>> keys);
	void load_keys(std::string path, std::vector<std::vector<std::string*>>* keys, Menu* menu,
		b2Vec2 pos, b2Vec2 indent, int character_size);
	void init_menu(std::string path_, Menu* object);
	Draw* draw;
public:
	Menu_Processing();
	void init(Draw* draw, b2Vec2* mouse_pos_,
		aux::Keyboard* keyboard_, bool* reload_);
	void step();
};



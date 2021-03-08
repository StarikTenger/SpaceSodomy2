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
	aux::Keyboard* keyboard;
	b2Vec2* mouse_pos;
	int current_id = 1;
	bool* reload;
	std::queue<int> events; // menu events
	std::map<int, std::string> text_fields_strings; // texts from text fields
	std::map<int, int> sliders_vals; // vals from sliders
	void init_menu(std::string path_, Menu* object);
	Draw* draw;
public:
	Menu_Processing();
	void init(Draw* draw, b2Vec2* mouse_pos_,
		aux::Keyboard* keyboard_, bool* reload_);
	void step();
};



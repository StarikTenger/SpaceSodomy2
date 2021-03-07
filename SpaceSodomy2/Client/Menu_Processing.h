#pragma once
#include<Menu/Menu.h>
#include<AuxLib/AuxLib.h>
#include<string>

class Menu_Processing {
private:
	Menu main_menu; // main menu object
	std::queue<int> events; // menu events
	std::map<int, std::string> text_fields_strings; // texts from text fields
	std::map<int, int> sliders_vals; // vals from sliders
	Draw* draw;
public:
	Menu_Processing();
	void init(std::string menu_config_path, Draw* draw, b2Vec2* mouse_pos_, aux::Keyboard* keyboard_);
	void step();
};



#pragma once
#include<Menu/Menu.h>
#include<AuxLib/AuxLib.h>
#include<string>

class Menu_Processing
{
private:
	Menu main_menu;
	std::queue<int> events;
	Draw* draw;
public:
	Menu_Processing();
	void init(std::string menu_config_path, Draw* draw, b2Vec2* mouse_pos_);
	void step();
};



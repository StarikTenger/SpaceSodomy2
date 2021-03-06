#pragma once
#include "Menu_Object.h"
#include "Button.h"
class Dropdown_list : public Menu_Object
{
private:
	std::vector<Button*> buttons;
	std::queue<int>* events;
public:
	bool dropdown_active = 0;

	void add_button(int id, std::string texture_name, float scale_x, float scale_y, sf::Color color, b2Vec2* mouse_pos);
	
	void set_events(std::queue<int>* events_);

	void step();
};


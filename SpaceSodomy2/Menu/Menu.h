#pragma once
#include "Button.h"
#include "Text_Field.h"
#include <vector>
#include <queue>

class Menu {
private:
	std::vector<Button*> buttons;
	std::vector<Text_Field*> text_fields;
	Draw* draw;
	std::queue<int>* events;
	bool active = 0;
public:
	Menu();
	Menu(Draw* draw_);

	//Set methods
	void set_draw(Draw* draw_);
	void set_active(bool active_);
	void set_events(std::queue<int>* events_);
	
	//Get methods
	Draw* get_draw();
	bool get_active();
	std::queue<int>* get_events();

	void add_text_field(int id, const wchar_t* text, std::string texture_name, float pos_x, float pos_y, float scale_x, float scale_y, sf::Color color, b2Vec2* mouse_pos, aux::Keyboard* keyboard);
	void add_button(int id, std::string texture_name, float pos_x, float pos_y, float scale_x, float scale_y, sf::Color color, b2Vec2* mouse_pos);
	void step();
};
#pragma once
#include "Button.h"
#include "Text_Field.h"
#include "Slider.h"
#include <vector>
#include <queue>

class Menu {
private:
	std::vector<Button*> buttons;
	std::vector<Text_Field*> text_fields;
	std::vector<Slider*> sliders;
	Draw* draw;
	std::queue<int>* events;
	std::map<int, std::string>* text_fields_strings;
	std::map<int, int>* sliders_vals;
	bool active = 0;
	bool last_mouse_status = 0;
	bool clicked = 0;
public:
	Menu();
	Menu(Draw* draw_);

	// Set methods
	void set_draw(Draw* draw_);
	void set_active(bool active_);
	void set_events(std::queue<int>* events_);
	void set_text_fields_strings(std::map<int, std::string>*);
	void set_sliders_vals(std::map<int, int>*);
	
	// Get methods
	Draw* get_draw();
	bool get_active();
	std::queue<int>* get_events();

	void add_text_field(int id, std::string text, std::string texture_name, b2Vec2 pos,
		b2Vec2 scale, sf::Color color, b2Vec2* mouse_pos, aux::Keyboard* keyboard);
	void add_button(int id, std::string texture_name, b2Vec2 pos, b2Vec2 scale,
		sf::Color color, b2Vec2* mouse_pos);
	void add_slider(int id, b2Vec2 pos, b2Vec2 axis_scale, b2Vec2 slider_scale,
		int min, int max, int val, b2Vec2* mouse_pos);
	void step();
};
#pragma once
#include "Button.h"
#include "Text_Field.h"
#include "Keyboard_Field.h"
#include "Constant_Text.h"
#include "Slider.h"
#include "Bar.h"
#include <vector>
#include <queue>

class Menu {
private:
	std::vector<Menu_Object*> objects;
	std::vector<Button*> buttons;
	std::vector<Text_Field*> text_fields;
	std::vector<Keyboard_Field*> keyboard_fields;
	std::vector<Constant_Text*> constant_texts;
	std::vector<Slider*> sliders;
	std::vector<Bar*> bars;
	Draw* draw;
	std::queue<int>* events;
	bool active = 0;
	bool last_mouse_status = 0;
	bool clicked = 0;
public:
	Menu();
	Menu(Draw* draw_);

	bool text_field_active = 0;

	// Set methods
	void set_draw(Draw* draw_);
	void set_active(bool active_);
	void set_events(std::queue<int>* events_);
	
	// Get methods
	Draw* get_draw();
	bool get_active();
	std::queue<int>* get_events();

	Bar* add_bar(int id, int use_window_cords, b2Vec2 pos, b2Vec2 scale, float character_size, sf::Color front_color,
		sf::Color back_color, int max, float val, int critical);
	Menu_Object* add_image(int id, std::string texture_name, b2Vec2 pos, int use_window_cords, b2Vec2 scale,
		b2Vec2* mouse_pos, bool use_image_scale);
	Text_Field* add_text_field(int id, std::string text, std::string texture_name, b2Vec2 pos, int use_window_cords,
		int character_size, sf::Color color, int align, b2Vec2* mouse_pos, aux::Keyboard* keyboard);
	Keyboard_Field* add_keyboard_field(int id, std::string text, std::string texture_name, b2Vec2 pos, int use_window_cords,
		int character_size, sf::Color color, int align, b2Vec2* mouse_pos, aux::Keyboard* keyboard);
	Constant_Text* add_constant_text(int id, std::string text, b2Vec2 pos, int use_window_cords,
		int character_size, sf::Color color, int align, b2Vec2* mouse_pos, aux::Keyboard* keyboard);
	Button* add_button(int id, std::string texture_name, b2Vec2 pos, int use_window_cords, b2Vec2 scale,
		sf::Color color, b2Vec2* mouse_pos, bool use_image_scale);
	Slider* add_slider(int id, b2Vec2 pos, int use_window_cords, b2Vec2 axis_scale, b2Vec2 slider_scale,
		int min, int max, int val, b2Vec2* mouse_pos);
	void step();
};
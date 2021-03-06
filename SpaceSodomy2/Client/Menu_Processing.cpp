#include "Menu_Processing.h"

Menu_Processing::Menu_Processing() {}

void Menu_Processing::init(std::string menu_config_path, Draw* draw_, b2Vec2* mouse_pos_, aux::Keyboard* keyboard) {
	draw = draw_;
	main_menu.set_draw(draw);
	main_menu.set_active(1);
	main_menu.set_events(&events);
	std::ifstream comment_file(menu_config_path);
	std::stringstream file = aux::comment(comment_file);
	for (int i = 1; i < 6; i++)
	{
		std::string type;
		int typenum = 0;
		std::string texture_name;
		float pos_x, pos_y, scale_x, scale_y;
		float axis_width, axis_height, slider_width, slider_height;
		file >> type;
		if (type == "Button")
			typenum = 1;
		if (type == "TextField")
			typenum = 2;
		if (type == "Slider")
			typenum = 3;
		switch (typenum) {
		case 1:
			file >> texture_name;
			file >> pos_x >> pos_y >> scale_x >> scale_y;
			main_menu.add_button(i, texture_name, pos_x, pos_y, scale_x, scale_y, sf::Color::White, mouse_pos_);
			break;
		case 2:
			file >> texture_name;
			file >> pos_x >> pos_y >> scale_x >> scale_y;
			main_menu.add_text_field(i, L"Hello", texture_name, pos_x, pos_y, scale_x, scale_y, sf::Color::White, mouse_pos_, keyboard);
			break;
		case 3:
			file >> pos_x >> pos_y;
			file >> axis_width >>  axis_height >> slider_width >> slider_height;
			main_menu.add_slider(i, pos_x, pos_y, axis_width, axis_height, slider_width, slider_height, mouse_pos_);
			break;
		default:
			break;
		}
	}

	return;
}

void Menu_Processing::step() {
	main_menu.step();
	while (!events.empty()){
		switch (events.front())
		{
		case 1:
			main_menu.set_active(0);
			break;
		case 2:
			std::cout << "well, it works";
			break;
		case 3:
			draw->get_window()->close();
			break;
		default:
			break;
		}
		events.pop();
	}
}
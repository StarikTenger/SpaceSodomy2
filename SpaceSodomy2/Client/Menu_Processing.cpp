#include "Menu_Processing.h"

Menu_Processing::Menu_Processing() {}

void Menu_Processing::init(std::string menu_config_path, Draw* draw_) {
	draw = draw_;
	main_menu.set_draw(draw);
	main_menu.set_active(1);
	std::ifstream comment_file(menu_config_path);
	std::stringstream file = aux::comment(comment_file);
	for (int i = 1; i < 4; i++)
	{
		std::string texture_name;
		float pos_x, pos_y, scale_x, scale_y;
		file >> texture_name;
		file >> pos_x >> pos_y >> scale_x >> scale_y;
		main_menu.add_button(i, texture_name, pos_x, pos_y, scale_x, scale_y, sf::Color::White);
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
		case 3:
			draw->get_window()->close();
		default:
			break;
		}
		events.pop();
	}
}
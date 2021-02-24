#pragma once
#include <SFML/Graphics.hpp>

class Menu_Object
{
private:
	sf::Texture* texture;
	sf::Sprite* sprite;
	sf::Window* window;
	int pos_x = 0;
	int pos_y = 0;
	int scale_x = 1;
	int scale_y = 1;
	bool active = 0;
public:
	Menu_Object();
	Menu_Object(sf::Texture* texture_, sf::Sprite* sprite_, sf::Window* window_,int pos_x_, int pos_y_, int scale_x_, int scale_y_);

	//Get methods
	sf::Texture* get_texture();
	sf::Sprite* get_sprite();
	sf::Window* get_window();
	int get_pos_x();
	int get_pos_y();
	int get_scale_x();
	int get_scale_y();
	bool get_active();

	//Set methods
	void set_texture(sf::Texture* texture_);
	void set_sprite(sf::Sprite* sprite_);
	void set_window(sf::Window* window_);
	void set_pos_x(int pos_x_);
	void set_pos_y(int pos_y_);
	void set_scale_x(int scale_x_);
	void set_scale_y(int scale_y_);

	void primitive_step();
};


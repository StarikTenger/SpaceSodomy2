#pragma once
#include <SFML/Graphics.hpp> 
#include <string>

class Player {
private:
	int id;
	sf::Color color;
	std::string name;
public:
	Player();
	Player(int id_, sf::Color color_, std::string name_);

	int get_id();
	sf::Color get_color();
	std::string get_name();
	
	void set_id(int id_);
	void set_color(sf::Color color_);
	void set_name(std::string name_);
};
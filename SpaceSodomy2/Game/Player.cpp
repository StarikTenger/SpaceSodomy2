#include "Player.h"

Player::Player() {}
Player::Player(int id_, sf::Color color_, std::string name_) {
	id = id_;
	color = color_;
	name = name_;
}

int get_id() {
	return id;
}
sf::Color get_color() {
	return color;
}
std::string get_name() {
	return name;
}

void set_id(int id_) {
	id = id_;
}
void set_color(sf::Color color_) {
	color = color_;
}
void set_name(std::string name_) {
	name = name_;
}
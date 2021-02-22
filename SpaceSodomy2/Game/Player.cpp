#include "pch.h"
#include "Player.h"

Player::Player() {}
Player::Player(int id_, sf::Color color_, std::string name_) {
	id = id_;
	color = color_;
	name = name_;
}

int Player::get_id() {
	return id;
}
sf::Color Player::get_color() {
	return color;
}
std::string Player::get_name() {
	return name;
}

void Player::set_id(int id_) {
	id = id_;
}
void Player::set_color(sf::Color color_) {
	color = color_;
}
void Player::set_name(std::string name_) {
	name = name_;
}
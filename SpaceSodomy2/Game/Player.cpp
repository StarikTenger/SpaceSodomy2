#include "pch.h"
#include "Player.h"

Player::Player() {}
Player::Player(int id_, sf::Color color_, std::string name_) {
	set_id(id_);
	color = color_;
	name = name_;
}

// Get methods
bool Player::get_is_alive() {
	return is_alive;
}

sf::Color Player::get_color() {
	return color;
}
std::string Player::get_name() {
	return name;
}

Command_Module* Player::get_command_module() {
	return command_module;
}

Counter* Player::get_time_to_respawn() {
	return time_to_respawn;
}

int Player::get_deaths() {
	return deaths;
}

int Player::get_kills() {
	return kills;
}

// Set methods
void Player::set_is_alive(bool val) {
	if (!val) {
		// TODO: add respawn time to config
		this->time_to_respawn->set(3);
	}
	is_alive = val;
}

void Player::set_color(sf::Color color_) {
	color = color_;
}

void Player::set_name(std::string name_) {
	name = name_;
}

void Player::set_command_module(Command_Module* val) {
	command_module = val;
}

void Player::set_time_to_respawn(Counter* val) {
	time_to_respawn = val;
}

void Player::set_deaths(int deaths_) {
	deaths = deaths_;
}
void Player::set_kills(int kills_) {
	kills = kills_;
}

void Player::add_death() {
	deaths++;
}

void Player::add_kill() {
	kills++;
}

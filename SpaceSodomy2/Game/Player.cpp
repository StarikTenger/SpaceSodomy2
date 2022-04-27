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

std::string Player::get_gun_name() {
	return gun_name;
}

std::string Player::get_hull_name() {
	return hull_name;
}

std::string Player::get_left_module_name() {
	return left_module_name;
}
std::string Player::get_right_module_name() {
	return right_module_name;
}

CommandModule* Player::get_command_module() {
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

int Player::get_ping() {
	return ping;
}

ShipBrain* Player::get_brain() {
	return brain;
}

// Set methods
void Player::set_is_alive(bool val) {
	is_alive = val;
}

void Player::set_color(sf::Color color_) {
	color = color_;
}

void Player::set_name(std::string name_) {
	name = name_;
}

void Player::set_gun_name(std::string val) {
	gun_name = val;
}

void Player::set_hull_name(std::string val) {
	hull_name = val;
}

void Player::set_left_module_name(std::string val) {
	left_module_name = val;
}
void Player::set_right_module_name(std::string val) {
	right_module_name = val;
}

void Player::set_command_module(CommandModule* val) {
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

void Player::set_ping(int ping_) {
	ping = ping_;
}

void Player::set_brain(ShipBrain* _) {
	brain = _;
}

void Player::add_death() {
	deaths++;
}

void Player::add_kill() {
	kills++;
}

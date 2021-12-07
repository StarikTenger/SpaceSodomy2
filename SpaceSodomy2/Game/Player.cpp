#include "pch.h"
#include "Player.h"

Player::Player(int id_, int team_id_, sf::Color color_, std::string name_) : Agent(id_, team_id_, color_, name_) {}

// Get methods
bool Player::get_is_alive() {
	return is_alive;
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

Counter* Player::get_time_to_respawn() {
	return time_to_respawn;
}

int Player::get_ping() {
	return ping;
}

// Set methods
void Player::set_is_alive(bool val) {
	if (!val) {
		// TODO: add respawn time to config
		this->time_to_respawn->set(3);
	}
	is_alive = val;
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

void Player::set_time_to_respawn(Counter* val) {
	time_to_respawn = val;
}

void Player::set_ping(int ping_) {
	ping = ping_;
}

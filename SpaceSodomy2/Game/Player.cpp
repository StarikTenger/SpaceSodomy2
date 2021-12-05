#include "pch.h"
#include "Player.h"

int Team_Id::get_team_id() { return id.get_id(); }
void Team_Id::set_team_id(int _id) { id.set_id(_id); }


Combatant::Combatant(int id_, sf::Color color_) : iId(id_), color(color_) {}

bool Combatant::is_friendly_fire = true;

void Combatant::set_friendly_fire(bool val) { 
	Combatant::is_friendly_fire = val; 
}
int Combatant::get_deaths() { return deaths; }
int Combatant::get_kills() { return kills; }

void Combatant::set_deaths(int deaths_) { deaths = deaths_; }
void Combatant::set_kills(int kills_) { kills = kills_; }

void Combatant::add_death() { deaths++; }
void Combatant::add_kill() { kills++; }
void Combatant::rm_kill() { kills--; }


sf::Color Combatant::get_color() { return color; }
void Combatant::set_color(sf::Color color_) { color = color_; }

bool Combatant::is_hostile_to(Combatant* other) { return get_team_id() != other->get_team_id(); }
bool Combatant::is_deals_damage_to(Combatant* other) { return Combatant::is_friendly_fire || get_team_id() != other->get_team_id(); }



Player::Player(int id_, sf::Color color_, std::string name_) {
	set_id(id_);
	set_team_id(id_);
	set_color(color_);
	set_name(name_);
}

// Get methods
bool Player::get_is_alive() {
	return is_alive;
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

Command_Module* Player::get_command_module() {
	return command_module;
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

void Player::set_command_module(Command_Module* val) {
	command_module = val;
}

void Player::set_time_to_respawn(Counter* val) {
	time_to_respawn = val;
}

void Player::set_ping(int ping_) {
	ping = ping_;
}

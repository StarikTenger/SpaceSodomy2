#include "pch.h"
#include "Agent.h"

int Team_Id::get_team_id() { return id.get_id(); }
void Team_Id::set_team_id(int _id) { id.set_id(_id); }


Combatant::Combatant(int id_, int team_id_, sf::Color color_, std::string name_) {
	set_id(id_);
	set_team_id(team_id_);
	set_color(color_);
	set_name(name_);
}

bool Combatant::is_friendly_fire = true;

void Combatant::set_friendly_fire(bool val) {
	Combatant::is_friendly_fire = val;
}

int Combatant::get_deaths() { return deaths; }
int Combatant::get_kills() { return kills; }
std::string Combatant::get_name() { return name; }


void Combatant::set_deaths(int deaths_) { deaths = deaths_; }
void Combatant::set_kills(int kills_) { kills = kills_; }
void Combatant::set_name(std::string name_) { name = name_; }


void Combatant::add_death() { deaths++; }
void Combatant::add_kill() { kills++; }
void Combatant::rm_kill() { if (kills > 0) kills--; }


sf::Color Combatant::get_color() { return color; }
void Combatant::set_color(sf::Color color_) { color = color_; }

bool Combatant::is_hostile_to(Combatant* other) { return !other || get_team_id() != other->get_team_id(); }
bool Combatant::is_deals_damage_to(Combatant* other) {
	return Combatant::is_friendly_fire || !other || get_team_id() != other->get_team_id();
}

Agent::Agent(int id_, int team_id_, sf::Color color_, std::string name_) : Combatant(id_, team_id_, color_, name_) {}

void Agent::set_command_module(Command_Module* val) {
	command_module = val;
}
Command_Module* Agent::get_command_module() {
	return command_module;
}

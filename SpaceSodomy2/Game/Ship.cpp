#include "pch.h"
#include "Ship.h"

Ship::Ship() {}

Ship::~Ship() {}

// Get methods
Player* Ship::get_player() {
	return player;
}

Engine* Ship::get_engine() {
	return engine;
}

b2Body* Ship::get_body() {
	return body;
}

Gun* Ship::get_gun() {
	return gun;
}

Counter* Ship::get_hp() {
	return hp;
}

Counter* Ship::get_stamina() {
	return stamina;
}
Counter* Ship::get_energy() {
	return energy;
}
DamageReceiver* Ship::get_damage_receiver() {
	return damage_receiver;
}

Module* Ship::get_left_module() {
	return left_module;
}
Module* Ship::get_right_module() {
	return right_module;
}

Effects* Ship::get_effects() {
	return effects;
}

BonusSlot* Ship::get_bonus_slot() {
	return bonus_slot;
}

// Set methods
void Ship::set_player(Player* val) {
	player = val;
}

void Ship::set_engine(Engine* val) {
	engine = val;
}

void Ship::set_body(b2Body* val) {
	body = val;
}

void Ship::set_gun(Gun* val) {
	gun = val;
}

void Ship::set_hp(Counter* val) {
	hp = val;
}

void Ship::set_stamina(Counter* val) {
	stamina = val;
}
void Ship::set_energy(Counter* val) {
	energy = val;
}
void Ship::set_damage_receiver(DamageReceiver* val) {
	damage_receiver = val;
}

void Ship::set_effects(Effects* val) {
	effects = val;
}

void Ship::set_bonus_slot(BonusSlot* val) {
	bonus_slot = val;
}


void Ship::set_left_module(Module* val) {
	left_module = val;
}
void Ship::set_right_module(Module* val) {
	right_module = val;
}
bool Ship::is_boost_active() {
	return get_player()->get_command_module()->get_command(CommandModule::BOOST)
		&& get_stamina()->get() > 0;
}

bool Ship::is_visible() {


	if (effects->get_effect(Effects::INVISIBILITY)->get_counter()->get() < b2_epsilon) {
		return true;
	}
	else {
		if (Effects::invis_flag == Effects::SHOW_NO) {
			return false;
		}
		bool res = false;
		if (Effects::invis_flag && Effects::SHOW_EFFECTS) {

			if ((effects->get_effect(Effects::BERSERK)->get_counter()->get() > b2_epsilon)
				|| (effects->get_effect(Effects::IMMORTALITY)->get_counter()->get() > b2_epsilon)
				|| (effects->get_effect(Effects::LASER)->get_counter()->get() > b2_epsilon)
				|| (effects->get_effect(Effects::CHARGE)->get_counter()->get() > b2_epsilon)) {
				res = true;
			}

		}
		if (Effects::invis_flag && Effects::SHOW_BOOST && is_boost_active()) {
			res = true;
		}
		return res;
	}
}

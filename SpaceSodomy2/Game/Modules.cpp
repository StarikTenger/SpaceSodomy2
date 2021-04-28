#include "pch.h"
#include "Modules.h"


Module::Type Module::get_named_type(std::string name) {
	if (name == "default") {
		return SHOTGUN;
	}
	if (name == "HP_UP") {
		return HP_UP;
	}
	if (name == "SHOTGUN") {
		return SHOTGUN;
	}
	if (name == "IMMORTALITY") {
		return IMMORTALITY;
	}
	if (name == "INVISIBILITY") {
		return INVISIBILITY;
	}
	if (name == "DASH") {
		return DASH;
	}
	std::cout << "unknown module name";
	return SHOTGUN;
}
Module::Type Module::get_type() {
    return type;
}

void Module::set_projectile_manager(Projectile_Manager* val) {
    projectile_manager = val;
}

float Module::get_strength() {
    return strength;
}
void Module::set_strength(float val) {
    strength = val;
}

void Module::set_type(Type val) {
    type = val;
}

Projectile_Manager* Module::get_projectile_manager() {
	return projectile_manager;
}


void Module::import_module_prototype(Module_Prototype* base) {
	type = base->type;
	strength = base->strength;
	stamina_cost = base->stamina_cost;
	recharge_time = base->recharge_time;
	effects_prototype = &base->effects_prototype;
}

void HpUp_Module::activate() {
	activate_side_effects();
	effects->get_effect(Effects::INSTANT_HP)->get_counter()->modify(strength);
}
void HpUp_Module::activate_side_effects() {
	activate_default_side_effects();
}


void Shotgun_Module::activate() {
    activate_side_effects(); 
	// TODO;
	Projectile_Def projectile_def;
	for (int i = -2; i < 3; i++) {
		float vel_val = 4;

		projectile_def.pos = body->GetPosition();
		projectile_def.vel = body->GetLinearVelocity();
		projectile_def.angle = body->GetAngle() + b2_pi / 10 * i;
		b2Vec2 delta_vel = vel_val * aux::angle_to_vec(projectile_def.angle);
		projectile_def.vel += delta_vel;
		projectile_def.player = player;
		projectile_def.damage = strength;
		projectile_def.radius = 0.25;
		projectile_def.mass = 0.02;
		projectile_def.hp = 20;
		projectile_def.effects_prototype = nullptr;
		// Recoil
		body->ApplyLinearImpulseToCenter(-projectile_def.mass * delta_vel, 1);
		get_projectile_manager()->create_projectile(projectile_def);
	}
}
void Shotgun_Module::activate_side_effects() {
    activate_default_side_effects();
}
void Immortality_Module::activate() {
    activate_side_effects();
	Effects_Prototype def;
	def.effects[Effects::IMMORTALITY].get_counter()->modify(strength);
	effects->update(&def);
}
void Immortality_Module::activate_side_effects() {
    activate_default_side_effects();
}
void Invisibility_Module::activate() {
    activate_side_effects();
	Effects_Prototype def;
	def.effects[Effects::INVISIBILITY].get_counter()->modify(strength);
	effects->update(&def);
}
void Invisibility_Module::activate_side_effects() {
    activate_default_side_effects();
}
void Dash_Module::activate() {
    activate_side_effects();
	body->ApplyLinearImpulseToCenter(strength * aux::angle_to_vec(body->GetAngle()), 1);
}
void Dash_Module::activate_side_effects() {
    activate_default_side_effects();
}

#include "pch.h"
#include "Gun.h"
#include <iostream>

void Gun::set_projectile_manager(Projectile_Manager* _projectile_manager) {
	projectile_manager = _projectile_manager;
}

void Gun::import_Gun_Prototype(Gun_Prototype def) {
	damage = def.damage;
	recharge_time = def.recharge_time;
	stamina_cost = def.stamina_cost;
	energy_cost = def.energy_cost;
	projectile_mass = def.projectile_mass;
	projectile_vel = def.projectile_vel;
	projectile_radius = def.projectile_radius;
	projectile_hp = def.projectile_hp;
	effects_prototype = &def.effect_prototype;
}

void Gun::activate() {

	Event_Def event_def;
	event_def.type = Event::SHOT;
	event_def.body = body;
	event_manager->create_event(event_def);
	// Apply BERSERK
	if (effects->get_effect(Effects::BERSERK)->get_counter()->get() > 0) {
		recharge_counter->set(recharge_time / effects->get_effect(Effects::BERSERK)->get_param("firing_rate_boost"));
		stamina->modify(-stamina_cost * effects->get_effect(Effects::BERSERK)->get_param("stamina_multiplier"));
	}
	else {
		activate_default_side_effects();
	}

	Projectile_Def projectile_def;

	float vel_val = projectile_vel;

	projectile_def.pos = body->GetPosition();
	projectile_def.vel = body->GetLinearVelocity();
	projectile_def.angle = body->GetAngle();
	b2Vec2 delta_vel = vel_val * aux::angle_to_vec(projectile_def.angle);
	projectile_def.vel += delta_vel;
	projectile_def.player = player;
	projectile_def.damage = damage;
	projectile_def.radius = projectile_radius;
	projectile_def.mass = projectile_mass;
	projectile_def.hp = projectile_hp;
	projectile_def.effects_prototype = effects_prototype;
	// Recoil
	// Apply BERSERK
	if (!(effects->get_effect(Effects::BERSERK)->get_counter()->get() > 0)) {
		body->ApplyLinearImpulseToCenter(-effects->get_effect(Effects::BERSERK)->get_param("recoil_modifier") * projectile_def.mass * delta_vel, 1);
	}
	projectile_manager->create_projectile(projectile_def);
}
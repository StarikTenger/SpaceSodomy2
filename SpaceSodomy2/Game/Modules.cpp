#include "pch.h"
#include "Modules.h"

std::map<int, std::string> Module::names = {
				{Type::HP_UP, "HP_UP"},
				{Type::SHOTGUN, "SHOTGUN"},
				{Type::IMMORTALITY, "IMMORTALITY"},
				{Type::INVISIBILITY, "INVISIBILITY"},
				{Type::DASH, "DASH"},
				{Type::FORCE, "FORCE"},
				{Type::BLINK, "BLINK"},
				{Type::COUNT, "COUNT"}
};

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
	if (name == "BLINK") {
		return BLINK;
	}
	if (name == "FORCE") {
		return FORCE;
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
float Module::get_param(std::string val) {
	return params[val];
}
void Module::set_param(std::string val, float num) {
	params[val] = num;
}

void Module::set_type(Type val) {
    type = val;
}

Projectile_Manager* Module::get_projectile_manager() {
	return projectile_manager;
}
void Module::set_game_objects(Game_Objects val) {
	environment = val;
}


void Module::import_module_prototype(Module_Prototype* base) {
	type = base->type;
	params = base->params;
	stamina_cost = base->stamina_cost;
	recharge_time = base->recharge_time;
	effects_prototype = &base->effects_prototype;
}

void HpUp_Module::activate() {
	activate_default_side_effects();
	effects->update(Effects::INSTANT_HP, params["heal"]);
}



void Shotgun_Module::activate() {
	activate_default_side_effects();
	Projectile_Def projectile_def;
	float num = params["bullet_num"];
	//std::cout << num;
	b2Vec2 impulse = {0,0};
	for (float i =  -(num / 2) + 0.5; i <= num / 2 - b2_epsilon; i++) {		
		float vel_val = params["velocity"];

		projectile_def.pos = body->GetPosition();
		projectile_def.vel = body->GetLinearVelocity();
		projectile_def.angle = body->GetAngle() + params["spread"] * (i);
		b2Vec2 delta_vel = vel_val * aux::angle_to_vec(projectile_def.angle);
		projectile_def.vel += delta_vel;
		projectile_def.player = player;
		projectile_def.damage = params["damage"];
		projectile_def.radius = params["radius"];
		projectile_def.mass = params["mass"];
		projectile_def.hp = params["bullet_hp"];
		projectile_def.effects_prototype = nullptr;
		// Recoil
		impulse += -projectile_def.mass * delta_vel;
		get_projectile_manager()->create_projectile(projectile_def);
	}
	body->ApplyLinearImpulseToCenter(impulse, 1);
}

void Immortality_Module::activate() {
	activate_default_side_effects();
	effects->update(Effects::IMMORTALITY, params["duration"]);
}
void Invisibility_Module::activate() {
	activate_default_side_effects();
	effects->update(Effects::INVISIBILITY, params["duration"]);
}

void Dash_Module::activate() {
	activate_default_side_effects();
	body->ApplyLinearImpulseToCenter(params["impulse"] * aux::angle_to_vec(body->GetAngle()), 1);
}

void Force_Module::activate() {
	activate_default_side_effects();
	std::set<Damage_Receiver*>& receivers = *environment.get_damage_receivers();
	for (auto receiver : receivers) {
		if ((body->GetWorldPoint({ 0,0 }) - receiver->get_body()->GetWorldPoint({ 0,0 })).Length() < params["radius"]) {
			receiver->damage(0, player);
			receiver->get_body()->ApplyLinearImpulseToCenter(
				- params["vel"] * receiver->get_body()->GetMass() * (body->GetWorldPoint({ 0,0 }) - receiver->get_body()->GetWorldPoint({ 0,0 })), 1);
			body->ApplyLinearImpulseToCenter(
				params["vel"] * receiver->get_body()->GetMass() * (body->GetWorldPoint({ 0,0 }) - receiver->get_body()->GetWorldPoint({ 0,0 })), 1);
		}
	}
}
void Blink_Module::activate() {
	std::set<Wall*>* walls = environment.get_walls();
	b2Vec2 newpos = body->GetWorldPoint({ 0,0 }) + params["distance"] * aux::angle_to_vec(body->GetAngle());
	for (auto wall = walls->begin(); wall != walls->end(); wall++) {
		Wall* wall_ = *wall;
		if (wall_->get_type() == Wall::SPIKED || wall_->get_type() == Wall::STANDART) {
			if (aux::is_in_polygon(newpos, wall_->get_vertices(), wall_->get_orientation())) {
				return;
			}
		}
	}
	activate_default_side_effects();
	body->SetTransform(newpos, body->GetAngle());
}

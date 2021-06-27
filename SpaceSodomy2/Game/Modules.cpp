#include "pch.h"
#include "Modules.h"

Module::Module(Module_Prototype* base) {
	type = base->type;
	stamina_cost = base->stamina_cost;
	energy_cost = base->energy_cost;
	recharge_time = base->recharge_time;
	effects_prototype = &base->effects_prototype;
}
std::map<int, std::string> Module::names = {
				{Type::HP_UP, "HP_UP"},
				{Type::SHOTGUN, "SHOTGUN"},
				{Type::IMMORTALITY, "IMMORTALITY"},
				{Type::INVISIBILITY, "INVISIBILITY"},
				{Type::DASH, "DASH"},
				{Type::FORCE, "FORCE"},
				{Type::MODULE_BLINK, "BLINK"},
				{Type::NONE, "NONE"},
	            {Type::ROCKET, "ROCKET"}
};

std::map<std::string, Module::Type> Module::named_types = {
				{"HP_UP", Type::HP_UP},
				{"SHOTGUN", Type::SHOTGUN},
				{"IMMORTALITY", Type::IMMORTALITY},
				{"INVISIBILITY", Type::INVISIBILITY},
				{"DASH", Type::DASH},
				{"FORCE", Type::FORCE},
				{"BLINK", Type::MODULE_BLINK},
				{"NONE", Type::NONE},
				{"ROCKET", Type::ROCKET},
				{"default", Type::NONE}
};

std::string Module::get_name_by_type(int val) {
	if (val >= COUNT) {
		std::cout << "Module::get_name_by_type() error: module #" << val << " not found\n";
		return "NONE";
	}
	return names[val];
}


Module::Type Module::get_type_by_name(std::string name) {
	if (!named_types.count(name)) {
		std::cout << "Module::get_type_by_name() error: module named " << name << " not found\n";
		return Module::NONE;
	}
	return named_types[name];
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

Rocket_Manager* Module::get_rocket_manager() {
	return rocket_manager;
}
void Module::set_rocket_manager(Rocket_Manager* val) {
	rocket_manager = val;
	//std::cout << "set\n";
}

None_Module::None_Module(Module_Prototype* base) : Module(base)  {
}

void None_Module::activate() {
}

HpUp_Module::HpUp_Module(Module_Prototype* base) : Module(base) {
	heal = base->params["heal"];
}

void HpUp_Module::activate() {
	activate_default_side_effects();
	effects->update(Effects::INSTANT_HP, params["heal"]);
}

Shotgun_Module::Shotgun_Module(Module_Prototype* base) : Module(base) {
	bullet_num = base->params["bullet_num"];
	velocity = base->params["velocity"];
	spread = base->params["spread"];
	damage = base->params["damage"];
	radius = base->params["radius"];
	mass = base->params["mass"];
	bullet_hp = base->params["bullet_hp"];
}

void Shotgun_Module::activate() {
	activate_default_side_effects();

	// Event
	event_manager->create_event(Event_Def(Event::MODULE_SHOTGUN, body));

	// Projectiles
	Projectile_Def projectile_def;
	float num = bullet_num;
	//std::cout << num;
	b2Vec2 impulse = {0,0};
	for (float i =  -(num / 2) + 0.5; i <= num / 2 - b2_epsilon; i++) {		
		float vel_val = velocity;

		projectile_def.pos = body->GetPosition();
		projectile_def.vel = body->GetLinearVelocity();
		projectile_def.angle = body->GetAngle() + spread * (i);
		b2Vec2 delta_vel = vel_val * aux::angle_to_vec(projectile_def.angle);
		projectile_def.vel += delta_vel;
		projectile_def.player = player;
		projectile_def.damage = damage;
		projectile_def.radius = radius;
		projectile_def.mass = mass;
		projectile_def.hp = bullet_hp;
		projectile_def.effects_prototype = nullptr;
		// Recoil
		impulse += -projectile_def.mass * delta_vel;
		get_projectile_manager()->create_projectile(projectile_def);
	}
	body->ApplyLinearImpulseToCenter(impulse, 1);
}

Immortality_Module::Immortality_Module(Module_Prototype* base) : Module(base) {
	duration = base->params["duration"];
}

void Immortality_Module::activate() {
	activate_default_side_effects();
	effects->update(Effects::IMMORTALITY, duration);
}

Invisibility_Module::Invisibility_Module(Module_Prototype* base) : Module(base) {
	duration = base->params["duration"];
}

void Invisibility_Module::activate() {
	activate_default_side_effects();
	effects->update(Effects::INVISIBILITY, duration);
}

Dash_Module::Dash_Module(Module_Prototype* base) : Module(base) {
	vel = base->params["vel"];
}

void Dash_Module::activate() {
	// Event
	event_manager->create_event(Event_Def(Event::MODULE_DASH, body));

	activate_default_side_effects();
	body->ApplyLinearImpulseToCenter(vel * body->GetMass() * aux::angle_to_vec(body->GetAngle()), 1);
}

Force_Module::Force_Module(Module_Prototype* base) : Module(base) {
	radius = base->params["radius"];
	vel = base->params["vel"];
	crit_mass = base->params["crit_mass"];
}

void Force_Module::activate() {
	activate_default_side_effects();
	std::set<Damage_Receiver*>& receivers = *environment.get_damage_receivers();
	for (auto receiver : receivers) {
		if ((body->GetWorldPoint({ 0,0 }) - receiver->get_body()->GetWorldPoint({ 0,0 })).Length() < radius) {
			receiver->damage(0, player);
			b2Vec2 unit = (receiver->get_body()->GetWorldPoint({ 0,0 }) - body->GetWorldPoint({ 0,0 }));
			unit.Normalize();
			b2Vec2 impulse = vel * (std::min( std::min(receiver->get_body()->GetMass(), body->GetMass()), crit_mass)) * unit;
			receiver->get_body()->ApplyLinearImpulseToCenter(impulse, 1);
			body->ApplyLinearImpulseToCenter(-impulse, 1);
		}
	}
	Event_Def event_def;
	event_def.pos = body->GetPosition();
	event_def.type = Event::MODULE_FORCE;
	event_manager->create_event(event_def);
}
Blink_Module::Blink_Module(Module_Prototype* base) : Module(base) {
	distance = base->params["distance"];
}
void Blink_Module::activate() {
	std::set<Wall*>* walls = environment.get_walls();
	b2Vec2 newpos = body->GetWorldPoint({ 0,0 }) + distance * aux::angle_to_vec(body->GetAngle());
	for (auto wall = walls->begin(); wall != walls->end(); wall++) {
		Wall* _wall = *wall;
		if (_wall->get_type() == Wall::SPIKED || _wall->get_type() == Wall::STANDART) {
			if (aux::is_in_polygon(newpos, _wall->get_vertices(), _wall->get_orientation())) {
				return;
			}
		}
	}
	// Event
	event_manager->create_event(Event_Def(Event::MODULE_BLINK, nullptr, body->GetPosition()));

	activate_default_side_effects();
	body->SetTransform(newpos, body->GetAngle());
}

Rocket_Module::Rocket_Module(Module_Prototype* base) : Module(base) {
	force_linear = base->params["force_linear"];
	hp = base->params["hp"];
	stamina = base->params["stamina"];
	stamina_recovery = base->params["stamina_recovery"];
	range = base->params["range"];
	bin_search_accuracy = base->params["bin_search_accuracy"];
	radius = base->params["radius"];
	mass = base->params["mass"];
	blast_radius = base->params["blast_radius"];
	damage = base->params["damage"];
	blast_force = base->params["blast_force"];
	rotation_factor = base->params["rotation_factor"];
}
void Rocket_Module::activate() {
	// Event
	event_manager->create_event(Event_Def(Event::MODULE_ROCKET, body));

	activate_default_side_effects();
	Rocket_Def def;
	def.base.force_linear = force_linear;
	def.base.hp = hp;
	def.base.stamina = stamina;
	def.base.stamina_recovery = stamina_recovery;
	def.base.range = range;
	def.base.bin_search_accuracy = bin_search_accuracy;
	def.base.radius = radius;
	def.base.mass = mass;
	def.base.blast_radius = blast_radius;
	def.base.damage = damage;
	def.base.blast_force = blast_force;
	def.player = player;
	def.angle = body->GetAngle();
	def.angle_vel = body->GetAngularVelocity() * rotation_factor;
	def.pos = body->GetWorldPoint({ 0,0 });
	def.vel = body->GetLinearVelocity();
	rocket_manager->create_rocket(def);
}
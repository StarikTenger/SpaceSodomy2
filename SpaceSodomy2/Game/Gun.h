#pragma once
#include <AuxLib/AuxLib.h>
#include "Active_Module.h"
#include "Projectile_Manager.h"
#include "Effects.h"
#include "Event.h"

struct Gun_Prototype;

class Gun : public Active_Module {
protected:
	float damage = 20.f;
	float projectile_mass = 0.05f;
	float projectile_vel = 10.f;
	float projectile_radius = 0.2f;
	float projectile_hp = 20.f;
	Projectile_Manager* projectile_manager = nullptr;
public:
	void set_projectile_manager(Projectile_Manager*);
	void import_Gun_Prototype(Gun_Prototype);
	void activate() override;
};

struct Gun_Prototype {
	float damage = 20.f;
	unsigned char alias = 1;
	float recharge_time = 0.5f;
	float stamina_cost = 10.f;
	float energy_cost = 0.f;
	float projectile_mass = 0.05f;
	float projectile_vel = 10.f;
	float projectile_radius = 0.2f;
	float projectile_hp = 20.f;
	Effects_Prototype effect_prototype;
};
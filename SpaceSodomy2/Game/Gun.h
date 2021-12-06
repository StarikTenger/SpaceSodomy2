#pragma once
#include <AuxLib/AuxLib.h>
#include "ActiveModule.h"
#include "ProjectileManager.h"
#include "Effects.h"
#include "Event.h"

struct Gun_Prototype;

class Gun : public ActiveModule {
protected:
	float damage = 20;
	float projectile_mass = 0.05;
	float projectile_vel = 10;
	float projectile_radius = 0.2;
	float projectile_hp = 20;
	ProjectileManager* projectile_manager = nullptr;
public:
	void set_projectile_manager(ProjectileManager*);
	void import_Gun_Prototype(Gun_Prototype);
	void activate() override;
};

struct Gun_Prototype {
	float damage = 20;
	unsigned char alias = 1;
	float recharge_time = 0.5;
	float stamina_cost = 10;
	float energy_cost = 0;
	float projectile_mass = 0.05;
	float projectile_vel = 10;
	float projectile_radius = 0.2;
	float projectile_hp = 20;
	Effects_Prototype effect_prototype;
};
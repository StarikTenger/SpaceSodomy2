#pragma once
#include <AuxLib/AuxLib.h>
#include "Active_Module.h"
#include "Gun_Def.h"
#include "Projectile_Manager.h"

class Gun : public Active_Module {
protected:
	float damage = 20;
	float projectile_mass = 0.05;
	float projectile_vel = 10;
	float projectile_radius = 0.2;
	Projectile_Manager* projectile_manager = nullptr;
public:
	void set_projectile_manager(Projectile_Manager*);
	void import_gun_def(Gun_Def);
	void activate() override;
};


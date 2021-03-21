#pragma once
#include <AuxLib/AuxLib.h>
#include "Active_Module.h"
#include "Projectile_Manager.h"

class Gun : public Active_Module {
protected:
	float damage = 20;
	Projectile_Manager* projectile_manager = nullptr;
public:
	void set_projectile_manager(Projectile_Manager*);
	void activate() override;
};


#pragma once
#include "Active_Module.h"

class Gun : public Active_Module {
private:
	float damage = 1;
public:
	void activate();
};


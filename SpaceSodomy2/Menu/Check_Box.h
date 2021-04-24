#pragma once
#include "Menu_Object.h"
class Check_Box : public Menu_Object {
private:
	bool state = false;
public:
	Check_Box();

	void set_state(bool state_);
	void change_state();

	bool get_state();

	void step();
};


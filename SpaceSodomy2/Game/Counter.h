#pragma once
#include <box2d/box2d.h>
#include "iId.h"

class Counter : public iId{
private:
	float value = 0;
	float max_value = 100;
	float delay = 0;
	float current_delay = 0;
	float change_vel = 0;
public:
	Counter();
	Counter(float _value);
	Counter(float _value, float _max_value);
	float get();
	float get_max();

	void set(float);
	void set_max(float);
	void set_delay(float);
	void restart_delay();
	void set_change_vel(float);
	void modify(float);
	void step(float dt);
};
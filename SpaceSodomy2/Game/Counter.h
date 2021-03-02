#pragma once
class Counter {
private:
	float value = 0;
	float change_vel = 0;

public:
	Counter();
	Counter(float _value);
	float get();
	void set(float);
	void set_change_vel(float);
	void modify(float);
	void step(float dt);
};
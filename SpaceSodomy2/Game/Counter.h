#pragma once
class Counter {
private:
	float value = 0;

public:
	Counter();
	float get();
	void set(float _value);
	void modify(float delta);
};


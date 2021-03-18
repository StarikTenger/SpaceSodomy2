#pragma once
#include "Slider.h"
#include "Menu.h"

class Scroll_Bar : private Slider, public Menu {
private:
public:
	Scroll_Bar();

	void step();
};


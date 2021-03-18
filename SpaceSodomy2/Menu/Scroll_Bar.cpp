#include "pch.h"
#include "Scroll_Bar.h"

Scroll_Bar::Scroll_Bar() {}

void Scroll_Bar::step() {
	Menu::step();
	Slider::step();
}
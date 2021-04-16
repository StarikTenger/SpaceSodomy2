#pragma once
#include <Draw/Draw.h>
#include <Menu/Constant_Text.h>
#include <Menu/Bar.h>
#include <string>
class Loading_Screen {
private:
	Draw* draw;
	Bar progress_bar;
	Constant_Text loading_stage;
public:
	Loading_Screen(Draw* draw_);

	void step(int val, std::string stage);
};


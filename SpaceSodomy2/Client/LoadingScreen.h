#pragma once
#include <Draw/Draw.h>
#include <string>
#include<TGUI/TGUI.hpp>
#include<TGUI/Backend/SFML-Graphics.hpp>

class LoadingScreen {
private:
	Draw* draw;
	tgui::Gui* _gui;
	tgui::ProgressBar::Ptr progress_bar = tgui::ProgressBar::create();
	tgui::Label::Ptr loading_stage = tgui::Label::create();
public:
	LoadingScreen(Draw* draw_, tgui::Gui &gui);

	void step(int val, std::string stage);
	void close();
};


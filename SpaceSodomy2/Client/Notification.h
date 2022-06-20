#pragma once
#include <Audio/Audio.h>
#include <Draw/Draw.h>
#include <AuxLib/AuxLib.h>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
class Notification {
private:
	tgui::Gui* gui;
	std::string HUD_effect_name;
	Draw* draw;
	Audio* audio;
	std::string audio_effect_name;
public:
	Notification();
	Notification(tgui::Gui* _gui, std::string _HUD_effect_name, Draw* _draw, Audio* _audio, std::string _audio_effect_name);
	void play();
};


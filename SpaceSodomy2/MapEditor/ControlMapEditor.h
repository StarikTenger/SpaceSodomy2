#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Network/ClientNetwork.h>
#include <AuxLib/AuxLib.h>
#include <Draw/Draw.h>
#include <Audio/Audio.h>
#include <string>
#include <fstream>
#include <queue>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "GameEditor.h"
#include <Control/Control.h>

class ControlMapEditor : public Control {
private:
	// Zoom velocity
	float zoom_vel = 2;

public:
	// Contains game objects
	GameEditor game;
	// Contains window & other drawing stuff
	Draw draw;
	// TGUI object
	tgui::Gui gui;
	// Contains mouse position
	b2Vec2 mouse_pos;
	// Keyboard structure
	aux::Keyboard keyboard;

	void load_config(std::string path) override;
	void outer_step() override;
	void inner_step(int time_delta) override;
};


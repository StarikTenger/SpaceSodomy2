#pragma once
#include <Game/Counter.h>
#include <string>
#include <vector>
#include <fstream>
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
class Replay {
private:
	Counter replay_frame;
	float speed;
	std::vector<std::string> frames;
	bool replay_active = 0;
	float speed_backup = 1;
	float dt = 0.020;
public:
	Replay();
	Replay(std::string);

	// Set methods
	void set_replay_path(std::string path);
	void set_speed(float val);
	void set_replay_active(bool val);
	void play_button(tgui::Gui& gui);

	// Get methods
	std::string get_cur_frame();
	Counter* get_replay_frame();
	bool get_replay_active();
	float get_seconds();

	void step(float _dt);
};


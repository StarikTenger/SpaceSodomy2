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
	// Time position
	Counter playback_time;
	// Playbacck speed
	float speed = 0;
	// All frames of replay as strings
	std::vector<std::string> frames;
	// Is game in replay mode
	bool replay_active = 0;
	// Is paused
	bool is_paused = 0;
	// To restore speed after unpause
	float speed_backup = 1;
	// Atom of time
	float dt = 0.020;
	// Amount of frames
	int frame_number = 0;
public:
	Replay();
	Replay(std::string);

	// This function is triggered when play-buttob or specific key is pressed
	void play_button(tgui::Gui& gui);

	// Set methods
	void set_replay_path(std::string path);
	void set_speed(float val);
	void set_replay_active(bool val);
	void set_frame(int frame_number);
	void set_frame_number(int frame_number);

	// Get methods
	int get_cur_frame_number();
	std::string get_cur_frame();
	Counter* get_replay_frame();
	bool get_replay_active();
	float get_seconds();
	int get_frame_number();

	void step(float _dt);
};


#pragma once
#include <Game/Counter.h>
#include <string>
#include <vector>
#include <fstream>
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
class Replay {
private:
	Counter replay_frame;
	float speed;
	std::vector<std::string> frames;
	bool replay_active = 0;
public:
	Replay();
	Replay(std::string);

	// Set methods
	void set_replay_path(std::string path);
	void set_speed(float val);
	void increase_speed();
	void decrease_speed();
	void set_replay_active(bool val);

	// Get methods
	std::string get_cur_frame();
	Counter* get_replay_frame();
	bool get_replay_active();
};


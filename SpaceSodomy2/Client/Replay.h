#pragma once
#include <Game/Counter.h>
#include <string>
#include <vector>
#include <fstream>
#include <box2d/box2d.h>
class Replay {
private:
	Counter replay_frame;
	float speed;
	std::vector<std::string> frames;
public:
	Replay();
	Replay(std::string);

	// Set methods
	void set_replay_path(std::string path);
	void increase_speed();
	void decrease_speed();

	// Get methods
	std::string get_cur_frame();
	Counter* get_replay_frame();
};


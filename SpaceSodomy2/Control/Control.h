#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <random>
#include <ctime>
#include <AuxLib/AuxLib.h>

class iControl {
protected:
	// 0 means the program is supposed to be stopped
	int running = 1;
	// Period of drawing & sending in ms
	int delay = 20;
	// Last time of activity
	int time_prev = 0;

public:
	virtual void load_config(std::string path) = 0; // loading configurations	
	virtual void outer_step() = 0;
	virtual void inner_step(int time_delta) = 0;

	int is_running();
	void step();
	void run();
};

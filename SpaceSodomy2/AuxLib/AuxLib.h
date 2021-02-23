#pragma once
#include <SFML/graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <string>
#include <sys/timeb.h>
#include <iostream>
#include <fstream>
#include <sstream>

class aux {
public:
	// Converts color from hsv to rgb
	static sf::Color from_hsv(float H, float S, float V);
	// Absolute time in ms
	static int get_milli_count();
	// Converts ms time to string format
	std::string get_time(int time);
	// Random funcs
	static void random_seed(int seed);
	static int random_int(int min, int max);
	static float random_float(float min, float max, int digits);
	static std::stringstream comment(std::ifstream& file);
};

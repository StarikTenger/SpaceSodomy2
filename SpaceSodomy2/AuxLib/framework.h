#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <SFML/graphics.hpp>
#include <cmath>
#include <cstdlib>
#include <string>
#include <sys/timeb.h>

namespace aux {
	// Converts color from hsv to rgb
	sf::Color from_hsv(float H, float S, float V);
	// Absolute time in ms
	int get_milli_count();
	// Converts ms time to string format
	std::string get_time(int time);
	// Random funcs
	void random_seed(int seed);
	int random_int(int min, int max);
	float random_float(float min, float max, int digits);
}
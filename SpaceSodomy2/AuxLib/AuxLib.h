#pragma once
#include <SFML/graphics.hpp>
#include <box2d/box2d.h>
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
	// Erases commented strings
	static std::stringstream comment(std::ifstream& file);
	// Converts bit mask to string
	static std::string mask_to_string(std::vector<int>, int digit = 16, char start = '0');
	// Converts string to bit max
	static std::vector<int> string_to_mask(std::string, int digit = 16, char start = '0');
	// Calculates polygon signed area 
	static float area(std::vector<b2Vec2>);
};

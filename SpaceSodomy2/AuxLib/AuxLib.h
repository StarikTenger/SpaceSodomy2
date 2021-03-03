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
#include <queue>

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
	// Returns normalized vector by angle
	static b2Vec2 angle_to_vec(float angle);
	// Transform Vector2f to b2vec2
	static b2Vec2 to_b2vec2(sf::Vector2f);
	// Transform b2vec2 to Vector2f
	static sf::Vector2f to_vector2f(b2Vec2);
	// Check if rectangle contains point
	static bool rect_contains(b2Vec2 center, b2Vec2 scale, b2Vec2 point);
	// Keyboard
	struct Keyboard {
		std::vector<int> state_current = std::vector<int>(sf::Keyboard::KeyCount);
		std::vector<int> state_prev = state_current;
		std::queue<wchar_t>* text_entered;
	};

};

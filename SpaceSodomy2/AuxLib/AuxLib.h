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
#include <direct.h>
#include <Windows.h>

class aux {
public:
	// Converts color from hsv to rgb
	static sf::Color from_hsv(float H, float S, float V);
	// Sets alpha chanel of color to 0
	static sf::Color make_transparent(sf::Color color);
	// Sets the opacity of given color
	static sf::Color set_opacity(sf::Color color, int value);
	// Absolute time in ms
	static int get_milli_count();
	// Converts ms time to string format
	std::string get_time(int time);
	// Random funcs
	static void random_seed(int seed);
	static int random_int(int min, int max);
	static float random_float(float min, float max, int digits);
	// Instantiate Templates
	static std::stringstream instantiate(std::stringstream& file);
	// Erases commented strings
	static std::stringstream comment(std::ifstream& file);
	// Converts bit mask to string
	static std::string mask_to_string(std::vector<int>, int digit = 16, char start = '0');
	// Converts string to bit max
	static std::vector<int> string_to_mask(std::string, int digit = 16, char start = '0');
	// Float to string with fixed digits
	static std::string float_to_string(float val, int precision);
	// Calculates polygon signed area 
	static float area(std::vector<b2Vec2>);
	// Returns normalized vector by angle
	static b2Vec2 angle_to_vec(float angle);
	// Transform Vector2f to b2vec2
	static b2Vec2 to_b2Vec2(sf::Vector2f);
	// Transform b2vec2 to Vector2f
	static sf::Vector2f to_Vector2f(b2Vec2);
	// Check if rectangle contains point
	static bool rect_contains(b2Vec2 center, b2Vec2 scale, b2Vec2 point);
	// Rotate point around point (angle in radians)
	static b2Vec2 rotate(b2Vec2 point, float angle);
	// Keyboard
	struct Keyboard {
		const std::vector<std::string> names =
		{ "A", "B", "C", "D", "E", "F", "G", "H",
		"I", "J", "K", "L", "M", "N", "O", "P", "Q",
		"R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"Num0", "Num1", "Num2", "Num3", "Num4", "Num5",
		"Num6", "Num7", "Num8", "Num9", "Escape", "LControl",
		"LShift", "LAlt", "LSystem", "RControl", "RShift",
		"RAlt", "RSystem", "Menu", "LBracket", "RBracket",
		"Semicolon", "Comma", "Period", "Quote", "Slash",
		"Backslash", "Tilde", "Equal", "Hyphen", "Space",
		"Enter", "Backspace", "Tab", "PageUp", "PageDown",
		"End", "Home", "Insert", "Delete", "Add", "Subtract",
		"Multiply", "Divide", "Left", "Right", "Up", "Down",
		"Numpad0", "Numpad1", "Numpad2", "Numpad3", "Numpad4",
		"Numpad5", "Numpad6", "Numpad7", "Numpad8", "Numpad9",
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9",
		"F10", "F11", "F12", "F13", "F14", "F15", "Pause" };
		std::vector<int> state_current = std::vector<int>(sf::Keyboard::KeyCount);
		std::vector<int> state_prev = state_current;
		std::queue<wchar_t>* text_entered;
	};
	// Get text scale
	static float get_text_max_height(sf::Text text);

	// Checks if the point is in the polygon. 
	static bool is_in_polygon(b2Vec2 point, const std::vector<b2Vec2>& polygon, bool is_outer);

	//
	static bool is_left_from_line(b2Vec2 point, b2Vec2 line_beg, b2Vec2 line_end);
	//
	static bool is_in_one_halfplane(b2Vec2 a, b2Vec2 b);

	// Distance from line given as two points. Is safe
	static float dist_from_line(b2Vec2 point, b2Vec2 segment_beg, b2Vec2 segment_end);
	// Distance from segment given as two points
	static float dist_from_segment(b2Vec2 point, b2Vec2 segment_beg, b2Vec2 segment_end);
	// Distance from polygon. Is always positive
	static float dist_from_polygon(b2Vec2 point, const std::vector<b2Vec2>& polygon);
	// Segment intersection
	static std::pair<bool, b2Vec2> segment_intersection(std::pair<b2Vec2, b2Vec2> segment_a, std::pair<b2Vec2, b2Vec2> segment_b);

	// Integer bounding box for polygons. 
	static b2Vec2 box_size(const std::vector<b2Vec2>& polygon);
	// Polygon origin as in SFML -- the left upper bounding point
	static b2Vec2 origin_pos(const std::vector<b2Vec2>& polygon);

	// Get direction
	static b2Vec2 direction(float angle);
	// Binpow
	static long long binpow(long long a, int b);

	// Packs textures into a single dir
    // WINDOWS ONLY
	static void mk_dir(std::string map_name);
	
	// Converts 2d vector to float angle
	static float vec_to_angle(b2Vec2 vec);

	// Gets resolution of the screen
	static std::pair<int, int> get_screen_resolution();


	// Python-like format (replaces * with custom word?)
	static std::string format(std::string base, std::string word);

	// Checks whether word fits the style of commands, COMMANDS must be caps
	static bool is_command(std::string word);

	// Accepts numbers < 255;
	static std::string write_int8(int val);
	static int read_int8(std::istream& in);

	// Caution: shorts larger than 127 * 255 wont work
	static std::string write_short(short val);
	static short read_short(std::istream& in);

	// Caution: ints larger than 127 * pow(255, 3) wont work
	static std::string write_int(int val);
	static int read_int(std::istream& in);

	// Caution: total val must fit approx 15000 / pow(10, precision)
	static std::string write_float(float val, unsigned precision);
	static float read_float(std::istream& in, unsigned precision);

	// To ban bad names, for exapmle, |_\/|_  C|\|
	static std::string censor_name(std::string);
};
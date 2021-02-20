// AuxLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"

// Converts color from hsv to rgb
sf::Color aux::from_hsv(float H, float S, float V) {
	float C = S * V;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = V - C;
	float Rs, Gs, Bs;

	if (H >= 0 && H < 60) {
		Rs = C;
		Gs = X;
		Bs = 0;
	}
	else if (H >= 60 && H < 120) {
		Rs = X;
		Gs = C;
		Bs = 0;
	}
	else if (H >= 120 && H < 180) {
		Rs = 0;
		Rs = 0;
		Gs = C;
		Bs = X;
	}
	else if (H >= 180 && H < 240) {
		Rs = 0;
		Gs = X;
		Bs = C;
	}
	else if (H >= 240 && H < 300) {
		Rs = X;
		Gs = 0;
		Bs = C;
	}
	else {
		Rs = C;
		Gs = 0;
		Bs = X;
	}

	sf::Color color;
	color.r = (Rs + m) * 255;
	color.g = (Gs + m) * 255;
	color.b = (Bs + m) * 255;
	return color;
}

// Absolute time in ms
int aux::get_milli_count() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

// Converts ms time to string format
std::string aux::get_time(int time) {
	std::string milliSeconds = std::to_string(time % 1000);
	time /= 1000;
	std::string seconds = std::to_string(time % 60);
	time /= 60;
	std::string minutes = std::to_string(time);

	milliSeconds = milliSeconds.substr(0, 1);
	while (seconds.size() < 2)
		seconds = "0" + seconds;
	while (minutes.size() < 2)
		minutes = "0" + minutes;

	return minutes + ":" + seconds;
}

// Random funcs
void aux::random_seed(int seed) {
	std::srand(seed);
}
int aux::random_int(int min, int max) {
	return std::rand() % (max + 1 - min) + min;
}
float aux::random_float(float min, float max, int digits) {
	float k = pow(10, (float)digits);
	return random_int(int(min * k), int(max * k)) / k;
}
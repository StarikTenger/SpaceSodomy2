// AuxLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "AuxLib.h"

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

std::stringstream aux::comment(std::ifstream& file) {
	std::stringstream newFile; // file without comments
	std::string elem; // current input

	while (file >> elem) {
		if (elem == "#") { // Comment
			std::string input = "";
			std::getline(file, input);
		}
		else {
			newFile << elem << " ";
		}
	}

	return newFile;
}

std::string aux::mask_to_string(std::vector<int> vec, int digit, char start) {
	int val = 0;
	for (int i = 0, d = 1; i < vec.size(); i++, d *= 2) {
		val += vec[i] * d;
	}
	//std::cout << "VAL " << val << "\n";
	std::string str = "#";
	for (; val; val /= digit) {
		str += (char)(start + val % digit);
	}
	return str;
}

std::vector<int> aux::string_to_mask(std::string str, int digit, char start) {
	int val = 0;
	for (int i = 1, d = 1; i < str.size(); i++, d *= digit) {
		val += (str[i] - start) * d;
	}

	std::vector<int> vec;
	for (; val; val /= 2) {
		vec.push_back(val % 2);
	}
	return vec;
}

float aux::area(std::vector<b2Vec2> vertices) {
	float val = 0;
	for (int i = 0; i < vertices.size(); i++) {
		int j = (i + 1) % vertices.size();
		val += vertices[i].x * vertices[j].y;
		val -= vertices[i].y * vertices[j].x;
	}
	return val / 2;
}

b2Vec2 aux::to_b2Vec2(sf::Vector2f val) {
	return b2Vec2(val.x, val.y);
}
sf::Vector2f aux::to_Vector2f(b2Vec2 val) {
	return sf::Vector2f(val.x, val.y);
}

bool aux::rect_contains(b2Vec2 center, b2Vec2 scale, b2Vec2 point) {
	scale.x /= 2;
	scale.y /= 2;
	b2Vec2 begin = center - scale, end = center + scale;
	return ((point.y > begin.y) && (point.x > begin.x) && (point.y < end.y) && (point.x < end.x));
}

b2Vec2 aux::rotate(b2Vec2 center, b2Vec2 point, float angle) {
	b2Vec2 point1;
	point -= center;
	point1.x = point.x * cos(angle) - point.y * sin(angle);
	point1.y = point.x * sin(angle) + point.y * cos(angle);
	point1 += center;
	return point1;
}

b2Vec2 aux::angle_to_vec(float angle) {
	return b2Vec2(cos(angle), sin(angle));
}


float aux::get_text_max_height(sf::Text text) {
	size_t character_size = text.getCharacterSize();
	sf::Font font = *text.getFont();
	std::string text_string = text.getString().toAnsiString();
	bool bold = (text.getStyle() & sf::Text::Bold);
	size_t width = 0;
	size_t max_height = 0;

	for (size_t x = 32; x < 256; x++)
	{
		sf::Uint32 character = char(x);
	
		const sf::Glyph& current_glyph = font.getGlyph(character, character_size, bold);
				size_t height = current_glyph.bounds.height;
		width += current_glyph.bounds.width;
		if (max_height < height)
			max_height = height;
	}
	return max_height;
}

std::string aux::to_string(float value) {
	std::stringstream s;
	s << value;
	return s.str();
}
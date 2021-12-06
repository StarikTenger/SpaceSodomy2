// AuxLib.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "AuxLib.h"
#include <iomanip>
#include <sstream>

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

sf::Color aux::make_transparent(sf::Color color) {
	color.a = 0;
	return color;
}

sf::Color aux::set_opacity(sf::Color color, int value) {
	color.a = value;
	return color;
}

// Absolute time in ms
int aux::get_milli_count() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

// Binpow
long long aux::binpow(long long a, int b) {
	if (b == 0)
		return 1;
	long long ans = binpow(a, b / 2);
	return ans * ans * ((b % 2) ? a : 1);
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
	float k = binpow(10, digits);
	return float(random_int(int(min * k), int(max * k))) / k;
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

	return instantiate(newFile);
}

std::stringstream aux::instantiate(std::stringstream& file) {
	std::deque<std::string> templ;
	std::stringstream newFile; //Ans
	std::string elem;

	while (file >> elem) {
		if (elem == "TEMPLATE") {
			templ.clear();
			file >> elem;
			if (elem == "LINK") {
				file >> elem;
				std::cout << "Following text template link " << elem << '\n';
				std::ifstream file(elem);
				std::stringstream list = (aux::comment(file));
				while (list >> elem) {
					templ.emplace_back(elem);
				}
				continue;
			}
			else if (elem == "[") {
				file >> elem;
				while (elem != "]") {
					templ.emplace_back(elem);
					file >> elem;
				}
				continue;
			}
			else if (elem == "IFLINK") {
				std::string condition;
				file >> condition;
				if (!is_command(condition)) {
					std::cout << "incorrect text template syntax\n";
					continue;
				}
				std::string list_name;
				file >> list_name;
				std::cout << "Following text template iflink " << list_name << '\n';
				std::ifstream file(list_name);
				std::stringstream list = aux::comment(file);
				while (list >> elem) {
					if (elem == condition) {
						while (list >> elem) {
							if (elem == "END") {
								break;
							}
							else {
								templ.emplace_back(elem);
							}
						}
					}
				}
				continue;
			}
		}
		if (elem == "{") {
			std::deque<std::string> to_format;
			file >> elem;
			while (elem != "}") {
				to_format.emplace_back(elem);
				file >> elem;
			}
			for (int j = 0; j < templ.size(); j++) {
				for (int i = 0; i < to_format.size(); i++) {
					newFile << aux::format(to_format[i], templ[j]) << " ";
				}
				newFile << "\n";
			}
			continue;
		}
		newFile << elem << " ";
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

std::string aux::float_to_string(float val, int precision) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(precision) << val;
	return stream.str();
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

b2Vec2 aux::rotate(b2Vec2 point, float angle) {
	return b2Vec2(point.x * cos(angle) - point.y * sin(angle), point.x * sin(angle) + point.y * cos(angle));
}

b2Vec2 aux::angle_to_vec(float angle) {
	return b2Vec2(cos(angle), sin(angle));
}

float aux::get_text_max_height(sf::Text text) {
	return 0;
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


bool aux::is_in_polygon(b2Vec2 point, const std::vector<b2Vec2>& polygon, bool is_outer) {
	int rot_number = 0;
	for (int i = 0; i < polygon.size(); i++) {
		auto segment_beg = polygon[i];
		auto segment_end = polygon[(i + 1) % polygon.size()];
		if (segment_beg.y <= point.y) {
			if (segment_end.y > point.y) {
				if (is_left_from_line(point, segment_beg, segment_end)) {
					rot_number++;
				}
			}
		}
		else {
			if (segment_end.y <= point.y) {
				if (!is_left_from_line(point, segment_beg, segment_end)) {
					rot_number--;
				}
			}
		}
		
	}
	return (bool)rot_number == is_outer;
}

float aux::dist_from_line(b2Vec2 point, b2Vec2 line_beg, b2Vec2 line_end) {
	if ((line_end - line_beg).Length() > b2_epsilon) {
		return abs(b2Dot(line_beg - point, b2Cross((line_end - line_beg), 1))) / (line_end - line_beg).Length();
	}
	else {
		return (line_beg - point).Length();
	}
}

float aux::dist_from_segment(b2Vec2 point, b2Vec2 segment_beg, b2Vec2 segment_end) {
	if (b2Dot(segment_beg - point, segment_beg - segment_end) > 0 !=
		b2Dot(segment_end - point, segment_beg - segment_end) > 0) {
		return dist_from_line(point, segment_beg, segment_end);
	}
	else {
		return std::min((segment_beg - point).Length(), (segment_end - point).Length());
	}
}

float aux::dist_from_polygon(b2Vec2 point, const std::vector<b2Vec2>& polygon) {
	float ans = dist_from_segment(point, polygon[0], polygon[polygon.size() - 1]);
	for (int i = 0; i < polygon.size() - 1; i++) {
		ans = std::min(ans,
			dist_from_segment(point, polygon[i], polygon[i + 1]));
	}
	return ans;
}

std::pair<bool, b2Vec2> aux::segment_intersection(std::pair<b2Vec2, b2Vec2> segment_a, std::pair<b2Vec2, b2Vec2> segment_b) {
	// I used formula from here: https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
	float x1 = segment_a.first.x;
	float x2 = segment_a.second.x;
	float x3 = segment_b.first.x;
	float x4 = segment_b.second.x;
	float y1 = segment_a.first.y;
	float y2 = segment_a.second.y;
	float y3 = segment_b.first.y;
	float y4 = segment_b.second.y;
	
	float denominator = ((x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4));
	float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / denominator;
	float u = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denominator;
	b2Vec2 intersection = segment_a.first + t * (segment_a.second - segment_a.first);
	bool is_cross = t >= 0 && t <= 1 && u >= 0 && u <= 1;
	return {is_cross, intersection};
}

b2Vec2 aux::box_size(const std::vector<b2Vec2>& polygon) {
	float max_x = polygon[0].x;
	for (int i = 0; i < polygon.size(); i++) {
		max_x = std::max(max_x, polygon[i].x);
	}
	float max_y = polygon[0].y;
	for (int i = 0; i < polygon.size(); i++) {
		max_y = std::max(max_y, polygon[i].y);
	}
	float min_x = polygon[0].x;
	for (int i = 0; i < polygon.size(); i++) {
		min_x = std::min(min_x, polygon[i].x);
	}
	float min_y = polygon[0].y;
	for (int i = 0; i < polygon.size(); i++) {
		min_y = std::min(min_y, polygon[i].y);
	}
	return b2Vec2(max_x - min_x, max_y - min_y);
}

b2Vec2 aux::origin_pos(const std::vector<b2Vec2>& polygon) {
	float min_x = polygon[0].x;
	for (int i = 0; i < polygon.size(); i++) {
		min_x = std::min(min_x, polygon[i].x);
	}
	float min_y = polygon[0].y;
	for (int i = 0; i < polygon.size(); i++) {
		min_y = std::min(min_y, polygon[i].y);
	}
	return b2Vec2(min_x, min_y);
}

b2Vec2 aux::direction(float angle) {
	return b2Vec2(cos(angle), sin(angle));
}

void aux::mk_dir(std::string path) {
	if (_mkdir((path).c_str())) {
		if (errno == EEXIST) {
			return;
		}
		std::cout << "Error: could not create directory: " << path << '\n';
	}
	std::cout << "Directory " << path << " created\n";
}

float aux::vec_to_angle(b2Vec2 vec) {
	return atan2(vec.y, vec.x);
}


bool aux::is_left_from_line(b2Vec2 point, b2Vec2 line_beg, b2Vec2 line_end) {
	return (b2Cross(line_end - line_beg, point - line_beg) < 0);
}

bool aux::is_in_one_halfplane(b2Vec2 a, b2Vec2 b) {
	return b2Dot(a, b) > 0;
}

std::pair<int, int> aux::get_screen_resolution() {
	return { 1920, 1080 };
}


std::string aux::format(std::string base, std::string word) {
	std::deque<std::string> temp;
	temp.emplace_back("");
	int cur = 0;
	for (int i = 0; i < base.size(); i++) {
		if (base[i] == '*') {
			cur++;
			temp.emplace_back("");
		}
		else {
			temp[cur] += base[i];
		}
	}
	std::string ans = temp[0];
	for (int i = 1; i < temp.size(); i++) {
		ans += word + temp[i];
	}
	return ans;
}

bool aux::is_command(std::string word) {
	for (auto i : word) {
		if (!isupper(i)) {
			return false;
		}
	}
	return true;
}

std::string aux::write_short(short val) {
	std::string out;
	out += (unsigned char)(abs(val) / 255) + 1 + 128 * (val < 0);
	out += (unsigned char)(abs(val) % 255) + 1;
	return out;
}
short aux::read_short(std::istream& in) {
	unsigned char first = in.get();
	unsigned char second = in.get();
	int16_t sign = -2 * (first > 128) + 1;
	int16_t f = (first % 128 - 1);
	int16_t s = second - 1;
	return sign * (f * 255 + s);
}

std::string aux::write_int(int val) {
	std::string out;
	out += (unsigned char)(abs(val) / (255*255*255)) + 1 + 128 * (val < 0);
	out += (unsigned char)((abs(val) / (255*255) ) % 255) + 1;
	out += (unsigned char)((abs(val) / 255) % 255) + 1;
	out += (unsigned char)(abs(val) % 255) + 1;
	return out;
}
int aux::read_int(std::istream& in) {
	unsigned char a, b, c, d;
	a = in.get();
	b = in.get();
	c = in.get();
	d = in.get();
	int sign = -2 * (a > 128) + 1;
	int a_ = (a % 128 - 1);
	int b_ = b - 1;
	int c_ = c - 1;
	int d_ = d - 1;
	return sign * (a_ * 255 * 255 * 255 + b_ * 255 * 255 + c_ * 255 + d_);
}

std::string aux::write_float(float val_, unsigned precision) {
	return write_short(std::trunc(val_ * pow(10, precision)));
}
float aux::read_float(std::istream& in, unsigned precision) {
	return (float)read_short(in) / pow(10, precision);
}

std::string aux::write_int8(int val) {
	std::string out;
	out += (unsigned char)(val + 1);
	return out;
}
int aux::read_int8(std::istream& in) {
	unsigned char val = in.get();
	return (int)val - 1;
}

std::string aux::censor_name(std::string str) {
	std::vector<std::string> euphemisms = {
		"[REDACTED]",
		"[EXPUNGED]",
		"[BANNED]",
		"[WINNIE_POOH]",
		"[DEFESTRATED]",
		"[FORBIDDEN]",
		"[PORRIDGE]",
		"[VETOED]",
		"[UNAUTHORIZED]",
		"[PROHIBITED]",
		"[ILLEGAL]",
		"[HRIUKNY]",
		"[OUTLAWED]",
		"[HARAM]"
	};
	if (str.size() > 20) {
		return euphemisms[std::abs(std::rand()) % euphemisms.size()];
	}
	for (auto ch : str) {
		if (!(isdigit(ch) || isupper(ch) || islower(ch) || ch == '_')) {
			return euphemisms[std::abs(std::rand()) % euphemisms.size()];
		}
	}
	return str;
}
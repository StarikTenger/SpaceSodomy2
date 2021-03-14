#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <Draw/Draw.h>
#include <string>
#include <map>
#include <fstream>

class Sound_Processing {
private:
	std::map<int, sf::Sound*> sound;
public:

	void step();
};


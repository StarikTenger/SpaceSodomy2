#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <fstream>
#include "Camera.h"
#include "FloatAnimation.h"

// Some primitives united in one object
struct Sprite {
private:
	std::vector<sf::RectangleShape> objects;
public:
	Sprite();
	void draw(sf::RenderWindow* window);
	void add_object(sf::RectangleShape object);
};


#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "Camera.h"

class Draw_Module {
private:
	sf::RenderWindow* window = nullptr;
	Camera cam;
public:
	Draw_Module();


	// Primirives
	/*void fillRect(b2Vec2 pos, b2Vec2 box, Color color);
	void fillCircle(b2Vec2 pos,float r, Color color);
	void strokeRect(b2Vec2 pos, b2Vec2 box, Color color);
	void line(b2Vec2 pos, b2Vec2 pos1, Color color);
	void image(std::string name, Vec2 pos, Vec2 box, float angle);
	void image(std::string name, Vec2 pos, Vec2 box, float angle, Color color);*/
};


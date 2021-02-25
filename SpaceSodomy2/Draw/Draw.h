#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <string>
#include <map>
#include <fstream>
#include "Camera.h"

class Draw {
private:
	sf::RenderWindow* window = nullptr;
	Camera cam;
	// Mathcehs textures and names
	std::map<std::string, sf::Texture*> textures;
	// Load single texture
	void load_texture(std::string name, std::string path_to_texture);
public:
	Draw();

	// Get methods
	sf::RenderWindow* get_window();
	Camera* get_camera();

	// Creates window
	sf::RenderWindow* create_window(int width = 600, int height = 420, std::string name = "window");

	// Load textures from file
	void load_textures(std::string path = "textures.conf");

	// Applies cameraborders
	void apply_camera();
	void apply_camera(b2Vec2 pos, float scale, float angle);

	// Displays the render
	void display();

	// Primirives
	void clear();
	void fill_rect(b2Vec2 pos, b2Vec2 box, sf::Color color);
	void stroke_rect(b2Vec2 pos, b2Vec2 box, sf::Color color);
	void fill_circle(b2Vec2 pos, float r, sf::Color color);
	void line(b2Vec2 start, b2Vec2 finish, sf::Color color);
	void image(std::string name, b2Vec2 pos, b2Vec2 box,
		float angle = 0, sf::Color color = sf::Color(255, 255, 255, 255));
};

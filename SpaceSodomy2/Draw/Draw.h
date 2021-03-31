#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include "Camera.h"
#include "Float_Animation.h"

class Draw {
private:
	sf::RenderWindow* window = nullptr;
	Camera cam;
	// Matches textures and names
	std::map<std::string, sf::Texture*> textures;
	// Matches fonts and names
	std::map<std::string, sf::Font*> fonts;
	// Animations
	std::set<Float_Animation*> animations;

public:
	// Load single texture
	void insert_texture(sf::Texture* tex, std::string name);
	void load_texture(std::string name, std::string path_to_texture);
	void load_font(std::string name, std::string path_to_font);
	void export_texture(std::string name, std::string path_to_texture);

	bool fullscreen = 0;
public:
	Draw();
	void step(float dt);

	// Get methods
	sf::RenderWindow* get_window();
	Camera* get_camera();
	bool isTextureExist(std::string name);
	sf::Texture* get_texture(std::string name);
	sf::Font* get_font(std::string name);

	// Set methods
	void set_camera(Camera _cam);

	// Creates window
	sf::RenderWindow* create_window(int width = 600, int height = 420, std::string name = "window");

	// Fullscreen
	void fullscreen_toggle();

	// Load textures from file
	void load_textures(std::string path = "textures.conf");
	// Load fonts from file
	void load_fonts(std::string path = "fonts.conf");


	// Applies cameraborders
	void apply_camera();
	void apply_camera(b2Vec2 pos, float scale, float angle);
	void camera_to_screen();

	// Displays the render
	void display();

	// Primitives
	void clear();
	void fill_rect(b2Vec2 pos, b2Vec2 box, sf::Color color, float angle = 0);
	void stroke_rect(b2Vec2 pos, b2Vec2 box, sf::Color color);
	void fill_circle(b2Vec2 pos, float r, sf::Color color);
	void line(b2Vec2 start, b2Vec2 finish, sf::Color color);
	void image(std::string name, b2Vec2 pos, b2Vec2 box,
		float angle = 0, sf::Color color = sf::Color(255, 255, 255, 255));
	void display_text(sf::Text* text);
	void text(std::string text, std::string font_name, b2Vec2 pos, int size, sf::Color color);

	// Animations
	void create_animation(Float_Animation);
	void draw_animations();

	bool make_polygonal_texture(const std::vector<b2Vec2>& polygon, bool is_outer,
		sf::Vector2f scale, std::string base_texture, std::string result_texture,
		float wall_width);

	void text(std::string text, std::string font_name, b2Vec2 pos, float size, float dir, sf::Color color);
};


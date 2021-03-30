#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <AuxLib/AuxLib.h>
#include <string>
#include <set>
#include <map>
#include <fstream>
#include <direct.h>
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
	// Load single texture
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

	// Primirives
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

	/// <summary>
	/// Makes a polygon-fitting texture. Its transparency is linear with distance for the polygon
	/// </summary>
	/// <param name="polygon">: the base polygon </param>
	/// <param name="is_outer">: the polygons orientation </param>
	/// <param name="scale">: the amount of pixels per unit distance </param>
	/// <param name="base_texture">: the base textures name</param>
	/// <param name="result_texture">: the results name</param>
	/// <param name="wall_width">: the width of the wall texture </param>
	/// returns true if the texture was created
	bool make_polygonal_texture(const std::vector<b2Vec2>& polygon, bool is_outer,
		sf::Vector2f scale, std::string base_texture, std::string result_texture,
		float wall_width);
	// Make a wall-fitting texture
	void make_wall_texture(const std::vector<b2Vec2>& wall, bool is_outer,
		std::string wall_texture, int wall_id, float wall_width, std::string map_name);

	//load map-specific wall textures
	void load_wall_textures(int walls_size, std::string wall_name, std::string map_name);

	void text(std::string text, std::string font_name, b2Vec2 pos, float size, float dir, sf::Color color);

	// Packs textures into a single dir
	// WINDOWS ONLY
	void mk_wall_dir(std::string map_name);
};


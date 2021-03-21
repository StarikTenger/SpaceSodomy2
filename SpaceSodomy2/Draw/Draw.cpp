// Draw.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include "Draw.h"
#include <iostream>

// PRIVATE //

void Draw::load_texture(std::string name, std::string path_to_texture) {
	sf::Texture* tex = new sf::Texture();
	textures.insert(std::make_pair(name, tex));
	textures[name]->loadFromFile(path_to_texture);
}
void Draw::load_font(std::string name, std::string path_to_font) {
	sf::Font* font = new sf::Font();
	fonts.insert(std::make_pair(name, font));
	fonts[name]->loadFromFile(path_to_font);
}

Draw::Draw() {}

sf::RenderWindow* Draw::get_window() {
	return window;
}

Camera* Draw::get_camera() {
	return &cam;
}

sf::Texture* Draw::get_texture(std::string name) {
	return textures[name];
}

sf::Font* Draw::get_font(std::string name) {
	return fonts[name];
}

void Draw::set_camera(Camera _cam) {
	cam = _cam;
}

// PUBLIC //

sf::RenderWindow* Draw::create_window(int width, int height, std::string name) {
	window = new sf::RenderWindow(sf::VideoMode(width, height), name);
	return window;
}

void Draw::load_textures(std::string path) {
	std::cout << "Start loading\n";
	std::ifstream input_file(path);
	auto file = aux::comment(input_file);
	while (file) {
		std::string name, path;
		file >> name >> path;
		std::cout << "loaded: " << name << " " << path << "\n";
		load_texture(name, path);
	}
	std::cout << "Finish loading\n";
}

void Draw::load_fonts(std::string path) {
	std::cout << "Start loading\n";
	std::ifstream input_file(path);
	auto file = aux::comment(input_file);
	while (file) {
		std::string name, path;
		file >> name >> path;
		std::cout << "loaded: " << name << " " << path << "\n";
		load_font(name, path);
	}
	std::cout << "Finish loading\n";
}

void Draw::apply_camera() {
	cam.set_borders(b2Vec2(window->getSize().x, window->getSize().y));
	cam.apply(window);
}

void Draw::apply_camera(b2Vec2 pos, float scale, float angle) {
	cam.set_pos(pos);
	cam.set_borders(b2Vec2(window->getSize().x, window->getSize().y));
	cam.set_scale(scale);
	cam.set_angle(angle);
	cam.apply(window);
}

void Draw::camera_to_screen() {
	cam.fit_to_screen(window);
	apply_camera();
}

void Draw::display() {
	window->display();
}

void Draw::clear() {
	window->clear();
}

void Draw::fill_rect(b2Vec2 pos, b2Vec2 box, sf::Color color, float angle) {
	sf::RectangleShape rectangle;
	rectangle.setOrigin(box.x / 2, box.y / 2);
	rectangle.setSize(sf::Vector2f(box.x, box.y));
	rectangle.setFillColor(color);
	rectangle.setPosition(pos.x, pos.y);
	rectangle.setRotation(angle);
	window->draw(rectangle);
}

void Draw::stroke_rect(b2Vec2 pos, b2Vec2 box, sf::Color color) {
	sf::RectangleShape rectangle;
	rectangle.setOrigin(box.x / 2, box.y / 2);
	rectangle.setSize(sf::Vector2f(box.x, box.y));
	rectangle.setOutlineColor(color);
	rectangle.setFillColor(sf::Color(0, 0, 0, 0));
	rectangle.setOutlineThickness(1);
	rectangle.setPosition(pos.x, pos.y);
	window->draw(rectangle);
}

void Draw::fill_circle(b2Vec2 pos, float r, sf::Color color) {
	sf::CircleShape circle;
	circle.setOrigin(r, r);
	circle.setRadius(r);
	circle.setFillColor(color);
	circle.setPosition(pos.x, pos.y);
	window->draw(circle);
}

void Draw::line(b2Vec2 start, b2Vec2 finish, sf::Color color) {
	sf::Vertex l[] =
	{
		sf::Vertex(sf::Vector2f(start.x, start.y)),
		sf::Vertex(sf::Vector2f(finish.x, finish.y))
	};
	l[0].color = l[1].color = color;
	window->draw(l, 2, sf::Lines);
}

void Draw::image(std::string name, b2Vec2 pos, b2Vec2 box,
	float angle, sf::Color color)
	{
	if (textures.find(name) == textures.end())
		return;
	sf::Texture& tex = *textures[name];
	b2Vec2 scale = { box.x / tex.getSize().x  , box.y / tex.getSize().y };
	sf::Sprite sprite;
	sprite.setOrigin(tex.getSize().x / 2, tex.getSize().y / 2);
	sprite.setTexture(tex);
	sprite.setScale(scale.x, scale.y);
	sprite.setPosition(pos.x, pos.y);
	sprite.setColor(sf::Color(
		std::min(255, (int)color.r),
		std::min(255, (int)color.g),
		std::min(255, (int)color.b),
		std::min(255, (int)color.a)));
	sprite.setRotation(angle * 180 / b2_pi);
	window->draw(sprite);
}

void Draw::display_text(sf::Text* text) {
	window->draw(*text);
}

void Draw::text(std::string text, std::string font_name, b2Vec2 pos, int size, sf::Color color) {
	sf::Text drawnText;
	drawnText.setFont(*fonts[font_name]);
	drawnText.setString(text);
	drawnText.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	drawnText.setCharacterSize(size);
	drawnText.setOrigin(floor(drawnText.getLocalBounds().width / 2), floor(drawnText.getLocalBounds().height));
	drawnText.setPosition(aux::to_Vector2f(pos));
	window->draw(drawnText);
}

void Draw::text(std::string text, std::string font_name, b2Vec2 pos, float size, float dir, sf::Color color) {
	sf::Text drawnText;
	drawnText.setFont(*fonts[font_name]);
	drawnText.setString(text);
	drawnText.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
	drawnText.setCharacterSize(60);
	drawnText.setScale(size / 2, size);
	drawnText.setOrigin(floor(drawnText.getLocalBounds().width / 2), floor(drawnText.getLocalBounds().height));
	drawnText.setPosition(aux::to_Vector2f(pos));
	drawnText.setRotation(dir * 180 / b2_pi);
	window->draw(drawnText);
}

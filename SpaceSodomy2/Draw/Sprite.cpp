#include "pch.h"
#include "Sprite.h"

Sprite::Sprite() {

}

void Sprite::draw(sf::RenderWindow* window) {
	for (auto& object : objects)
		window->draw(object);
}

void Sprite::add_object(sf::RectangleShape object) {
	objects.push_back(object);
	std::cout << "sprite object added " << objects.size() << "\n";
}

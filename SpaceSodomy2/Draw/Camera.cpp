#include "pch.h"
#include "Camera.h"

// Constructors
Camera::Camera() {}

// Get methods
b2Vec2 Camera::get_pos() {
	return pos;
}
b2Vec2 Camera::get_borders() {
	return borders;
}
float Camera::get_angle() {
	return angle;
}
float Camera::get_scale() {
	return scale;
}

// Set methods
void Camera::set_pos(b2Vec2 val) {
	pos = val;
}
void Camera::set_borders(b2Vec2 val) {
	borders = val;
}
void Camera::set_angle(float val) {
	angle = val;
}
void Camera::set_scale(float val) {
	scale = val;
}

void Camera::modify_scale(float val) {
	scale *= val;
}

void Camera::apply(sf::RenderWindow* window) {
	// Relative view
	sf::View view(sf::FloatRect(
		sf::Vector2f(pos.x - borders.x * 1 / scale / 2, pos.y - borders.y * 1 / scale / 2),
		sf::Vector2f(borders.x * 1 / scale, borders.y * 1 / scale)
	));
	view.setRotation((angle * 180 / b2_pi) + 90);
	window->setView(view);
}

void Camera::fit_to_screen(sf::RenderWindow* window) {
	pos = b2Vec2(0, 0);
	scale = 1;
	angle = 1.5 * b2_pi;
	borders = b2Vec2(aux::to_b2Vec2(sf::Vector2f(window->getSize())));
}

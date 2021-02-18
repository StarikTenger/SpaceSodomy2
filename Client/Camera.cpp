#include "Camera.h"

// Constructors
Camera::Camera() {}
// Get methods
b2Vec2 Camera::get_pos() {
	return pos;
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
void Camera::set_angle(float val) {
	angle = val;
}
void Camera::set_scale(float val) {
	scale = val;
}

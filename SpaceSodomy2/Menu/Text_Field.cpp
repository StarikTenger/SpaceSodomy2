#include "pch.h"
#include "Text_Field.h"

Text_Field::Text_Field() {
	set_use_picture_scale(0);
	text.setString("None");
	text.setFillColor(sf::Color::White);
	text.setCharacterSize(10);
}

// Get methods
std::string Text_Field::get_text() {
	return text.getString();
}
sf::Font Text_Field::get_font() {
	return *text.getFont();
}
sf::Color Text_Field::get_text_color() {
	return text.getFillColor();
}
unsigned int Text_Field::get_text_character_pixels_size() {
	return text.getCharacterSize();
}
b2Vec2 Text_Field::get_indent() {
	return indent;
}

// Set methods
void Text_Field::set_text(const wchar_t* text_) {
	text.setString(text_);
}
void Text_Field::set_font(sf::Font font_) {
	text.setFont(font_);
}
void Text_Field::set_text_color(sf::Color color_) {
	text.setFillColor(color_);
}
void Text_Field::set_text_character_pixel_size(unsigned int size_) {
	text.setCharacterSize(size_);
}
void Text_Field::set_indent(b2Vec2 indent_) {
	indent = indent_;
}

void Text_Field::step() {
	set_scale(aux::to_b2vec2(sf::Vector2f(text.getScale())));
	set_scale(get_scale() + indent);
	primitive_step();
	text.setPosition(aux::to_vector2f(get_pos()));
	text.setFont(*get_draw()->get_font("font"));
	get_draw()->display_text(&text);
}
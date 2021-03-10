#include "pch.h"
#include "Text_Field.h"

Text_Field::Text_Field() {
	set_use_picture_scale(0);
	text.setString(L"None");
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
bool Text_Field::get_keyboard_active() {
	return keyboard_active;
}

// Set methods
void Text_Field::set_text(std::string text_) {
	text.setString(text_);
}
void Text_Field::set_font(sf::Font font_) {
	font_setted = 1;
	text.setFont(font_);
	height = aux::get_text_max_height(text);
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
void Text_Field::set_keyboard_active(bool keyboard_active_) {
	keyboard_active = keyboard_active_;
}

void Text_Field::step() {
	if (!font_setted) { // if font isn't setted -> set standart font
		font_setted = 1;
		text.setFont(*(get_draw()->get_font("font")));
		height = aux::get_text_max_height(text);
	}
	// Text entering
	if (keyboard_active) {
		std::string str = text.getString();
		while (!get_keyboard()->text_entered->empty()) {
			wchar_t symbol = get_keyboard()->text_entered->front();
			get_keyboard()->text_entered->pop();
			if (symbol == 8 && !str.empty())
				str.pop_back();
			if (symbol >= 32)
				str.push_back(symbol);
		}
		text.setString(str);
	}
	// Set background params
	if (height < text.getGlobalBounds().height)
		height = text.getLocalBounds().height;;
	set_scale(b2Vec2(text.getLocalBounds().width, height));
	set_scale(get_scale() + indent);
	set_color(sf::Color(40, 40, 40, 255));
	if (keyboard_active)
		get_draw()->stroke_rect(get_pos() + b2Vec2(indent.x / 2.0, indent.y / 2.0), get_scale() + indent, sf::Color::White);
	primitive_step();
	//std::cout << "Text: " << get_active() << " " << keyboard_active << " " << text.getString().toAnsiString() << "\n";
	text.setPosition(aux::to_Vector2f(get_pos()) - sf::Vector2f(text.getLocalBounds().width / 2.0,
		height / 2.0));
	get_draw()->display_text(&text);
}
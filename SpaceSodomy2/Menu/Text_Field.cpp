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
	//height = aux::get_text_max_height(text);
}
void Text_Field::set_font_name(std::string font_name_) {
	font_name = font_name_;
	font_setted = false;
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
void Text_Field::set_text_scale(float scale) {
	text.setScale(scale * text.getScale());
}
void Text_Field::set_text_angle(float angle) {
	text.setRotation(angle);
}
void Text_Field::set_align(int align_) {
	align = align_;
}

void Text_Field::step() {
	primitive_step();
	if (!font_setted) { // if font isn't setted -> set standart font
		font_setted = 1;
		text.setFont(*(get_draw()->get_font(font_name)));
		//height = aux::get_text_max_height(text);
	}
	// Text entering
	if (keyboard_active && active_field) {
		while (!get_keyboard()->text_entered->empty()) {
			new_text = text.getString();
			wchar_t symbol = get_keyboard()->text_entered->front();
			if (keyboard_activated) {
				new_text = "";
				keyboard_activated = 0;
			}
			get_keyboard()->text_entered->pop();
			if (symbol == 8 && !new_text.empty())
				new_text.pop_back();
			if (symbol >= 32)
				new_text.push_back(symbol);
			if (symbol == 13)
			{
				keyboard_active = 0;
				break;
			}
			text.setString(new_text);
		}
	}
	else {
		keyboard_activated = 1;
	}
	// Set background params
	sf::Vector2f text_scale_backup = text.getScale();
	text.setScale((float)get_screen_mode() * text.getScale());
	set_scale(b2Vec2(text.getLocalBounds().width, text.getLocalBounds().height));
	set_scale(get_scale() + indent);
	set_color(sf::Color(40, 40, 40, 255));
	switch (align)
	{
	case 1:
		set_cur_pos(b2Vec2(get_pos().x + get_screen_mode() *  text.getLocalBounds().width / 2, get_pos().y));
		break;
	case 2:
		set_cur_pos(b2Vec2(get_pos().x - get_screen_mode() * text.getLocalBounds().width / 2, get_pos().y));
		break;
	default:
		set_cur_pos(get_pos());
		break;
	}
	if (keyboard_active && active_field)
		get_draw()->stroke_rect(get_cur_pos() + get_screen_mode() * (b2Vec2(indent.x / 2.0, indent.y / 2.0)),
			get_screen_mode() * (get_scale() + indent), sf::Color::White);
	text.setOrigin(sf::Vector2f(text.getLocalBounds().width / 2.0, text.getLocalBounds().height / 2.0));
	text.setPosition(aux::to_Vector2f(get_cur_pos()));
	get_draw()->display_text(&text);
	text.setScale(text_scale_backup);
}
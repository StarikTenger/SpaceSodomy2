#include "pch.h"
#include "Slider.h"

Slider::Slider() {
	axis_scale = { 200, 10 };
	slider_scale = { 20, 30 };

	text.setFillColor(sf::Color::White);

	set_image_active(0); // Menu_Object won't be rendered
}

void Slider::init() {
	cord = { get_pos().x , get_pos().y };

	// set axis params
	axis.setPosition(cord.x, cord.y);
	axis.setOrigin(0, axis_scale.y / 2);
	axis.setSize(aux::to_Vector2f(axis_scale));
	axis.setFillColor(sf::Color(63, 63, 63));
	// set slider params
	slider.setPosition(cord.x, cord.y);
	slider.setOrigin(slider_scale.x / 2, slider_scale.y / 2);
	slider.setSize(sf::Vector2f(slider_scale.x, slider_scale.y));
	slider.setFillColor(sf::Color(192, 192, 192));
}

sf::Text Slider::returnText(b2Vec2 pos_, std::string z, int fontSize)
{
	text.setCharacterSize(fontSize);
	text.setPosition(pos_.x, pos_.y);
	text.setString(z);
	return text;
}

void Slider::create(int min, int max)
{
	slider_value = min;
	min_value = min;
	max_value = max;
}

void Slider::logic(sf::RenderWindow* window)
{
	// Creating a new slider rect for logic
	sf::FloatRect SliderRect = axis.getGlobalBounds();
	if (SliderRect.height < slider.getGlobalBounds().height)
		SliderRect.height = slider.getGlobalBounds().height;
	// if slider was clicked -> slider active
	if (SliderRect.contains(mouse_pos_) && *get_clicked())
		slider_active = 1;
	// if slider has been deactivated -> slider inactive
	if (slider_active
		&& !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		slider_active = 0;
	// if slider active -> move slider pointer
	if (slider_active) {
		// Creating a new slider pointer pos_x
		auto new_pos_x = mouse_pos_.x;
		if (new_pos_x < cord.x)
			new_pos_x = cord.x;
		if (new_pos_x > cord.x + axis_scale.x)
			new_pos_x = cord.x + axis_scale.x;
		slider.setPosition(new_pos_x, cord.y);
		slider_value = (min_value + ((slider.getPosition().x - cord.x) / axis_scale.x * (max_value - min_value)));
	}
}

float Slider::get_slider_value() {
	return slider_value;
}

void Slider::set_slider_value(float value_)
{
	if (value_ >= min_value && value_ <= max_value)
	{
		slider_value = value_;
		float diff = max_value - min_value;
		float diff2 = value_ - min_value;
		float zzz = axis_scale.x / diff;
		float pos_x = zzz * diff2;
		pos_x += cord.x;
		slider.setPosition(pos_x, cord.y);
	}
}

void Slider::set_slider_percent_value(float percent_value_)
{
	if (percent_value_ >= 0 && percent_value_ <= 100) {
		slider_value = percent_value_ / 100 * max_value;
		slider.setPosition(cord.x + (axis_scale.x * percent_value_ / 100), cord.y);
	}
}

void Slider::draw(sf::RenderWindow* window)
{
	logic(window);
	window->draw(returnText({ cord.x - 10, cord.y + 5 }, std::to_string(min_value), 20));
	window->draw(axis);
	window->draw(returnText({ cord.x + axis_scale.x - 10, cord.y + 5 }, std::to_string(max_value), 20));
	window->draw(slider);
	window->draw(returnText({ slider.getPosition().x - slider_scale.x, 
		slider.getPosition().y - slider_scale.y }, std::to_string((int)slider_value), 15));
}

void Slider::set_axis_scale(b2Vec2 axis_scale_) {
	axis_scale = axis_scale_;
	axis.setOrigin(0, axis_scale.y / 2);
	axis.setSize(aux::to_Vector2f(axis_scale));
}
void Slider::set_slider_scale(b2Vec2 slider_scale_) {
	slider_scale = slider_scale_;
	slider.setOrigin(slider_scale.x / 2, slider_scale.y / 2);
	slider.setSize(aux::to_Vector2f(slider_scale));
}

b2Vec2 Slider::get_axis_scale() {
	return axis_scale;
}
b2Vec2 Slider::get_slider_scale() {
	return slider_scale;
}

void Slider::step() {
	set_scale(slider_scale);
	set_pos(aux::to_b2Vec2(slider.getPosition()));
	b2Vec2 mid = aux::to_b2Vec2(sf::Vector2f(get_draw()->get_window()->getSize()));
	mid.x /= 2;
	mid.y /= 2;
	mouse_pos_ = aux::to_Vector2f(*get_mouse_pos() - mid);
	std::cout << "Slider: " << get_active() << " " << slider.getGlobalBounds().left << " " << 
		slider.getGlobalBounds().top << " " << mouse_pos_.x << " " << mouse_pos_.y << " " <<
		slider.getGlobalBounds().contains(mouse_pos_) << " " <<
		sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) << " " << slider_active << "\n";
	primitive_step();
	if (!font_active) {
		text.setFont(*(get_draw()->get_font("neon")));
		font_active = 1;
	}
	draw(get_draw()->get_window());
}
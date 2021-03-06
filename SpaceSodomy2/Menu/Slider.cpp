#include "pch.h"
#include "Slider.h"

Slider::Slider() {
	axisWidth = 200;
	axisHeight = 10;
	sliderWidth = 20;
	sliderHeight = 30;

	text.setFillColor(sf::Color::White);

	set_image_active(0);
}

void Slider::init() {
	float x = get_pos().x;
	float y = get_pos().y;
	xCord = x;
	yCord = y;

	axis.setPosition(x, y);
	axis.setOrigin(0, axisHeight / 2);
	axis.setSize(sf::Vector2f(axisWidth, axisHeight));
	axis.setFillColor(sf::Color(63, 63, 63));
	slider.setPosition(x, y);
	slider.setOrigin(sliderWidth / 2, sliderHeight / 2);
	slider.setSize(sf::Vector2f(sliderWidth, sliderHeight));
	slider.setFillColor(sf::Color(192, 192, 192));
}

sf::Text Slider::returnText(int x, int y, std::string z, int fontSize)
{
	text.setCharacterSize(fontSize);
	text.setPosition(x, y);
	text.setString(z);
	return text;
}

void Slider::create(int min, int max)
{
	sliderValue = min;
	minValue = min;
	maxValue = max;
}

void Slider::logic(sf::RenderWindow& window)
{
	sf::FloatRect SliderRect = axis.getGlobalBounds();
	if (SliderRect.height < slider.getGlobalBounds().height)
		SliderRect.height = slider.getGlobalBounds().height;
	if (SliderRect.contains(mouse_pos_) && *get_clicked())
		slider_active = 1;
	if (slider_active
		&& !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		slider_active = 0;
	if (slider_active) {
		if (mouse_pos_.x >= xCord && mouse_pos_.x <= xCord + axisWidth)
		{
			auto new_pos_x = mouse_pos_.x;
			if (new_pos_x < xCord)
				new_pos_x = xCord;
			if (new_pos_x > xCord + axisWidth)
				new_pos_x = xCord + axisWidth;
			slider.setPosition(new_pos_x, yCord);
			sliderValue = (minValue + ((slider.getPosition().x - xCord) / axisWidth * (maxValue - minValue)));
		}
	}
}

float Slider::getSliderValue()
{
	return sliderValue;
}

void Slider::setSliderValue(float newValue)
{
	if (newValue >= minValue && newValue <= maxValue)
	{
		sliderValue = newValue;
		float diff = maxValue - minValue;
		float diff2 = newValue - minValue;
		float zzz = axisWidth / diff;
		float posX = zzz * diff2;
		posX += xCord;
		slider.setPosition(posX, yCord);
	}
}

void Slider::setSliderPercentValue(float newPercentValue)
{
	if (newPercentValue >= 0 && newPercentValue <= 100)
	{
		sliderValue = newPercentValue / 100 * maxValue;
		slider.setPosition(xCord + (axisWidth * newPercentValue / 100), yCord);
	}
}

void Slider::draw(sf::RenderWindow& window)
{
	logic(window);
	window.draw(returnText(xCord - 10, yCord + 5, std::to_string(minValue), 20));
	window.draw(axis);
	window.draw(returnText(xCord + axisWidth - 10, yCord + 5, std::to_string(maxValue), 20));
	window.draw(slider);
	window.draw(returnText(slider.getPosition().x - sliderWidth, slider.getPosition().y - sliderHeight,
		std::to_string((int)sliderValue), 15));
}

void Slider::setAxisWidth(int newAxisWidth) {
	axisWidth = newAxisWidth;
	axis.setSize(sf::Vector2f(axisWidth, axisHeight));
}
void Slider::setAxisHeight(int newAxisHeight) {
	axisHeight = newAxisHeight;
	axis.setOrigin(0, axisHeight / 2);
	axis.setSize(sf::Vector2f(axisWidth, axisHeight));
}
void Slider::setSliderWidth(int newSliderWidth) {
	sliderWidth = newSliderWidth;
	slider.setOrigin(sliderWidth / 2, sliderHeight / 2);
	slider.setSize(sf::Vector2f(sliderWidth, sliderHeight));
}
void Slider::setSliderHeight(int newSliderHeight) {
	sliderHeight = newSliderHeight;
	slider.setOrigin(sliderWidth / 2, sliderHeight / 2);
	slider.setSize(sf::Vector2f(sliderWidth, sliderHeight));
}

int Slider::getAxisWidth() {
	return axisWidth;
}
int Slider::getAxisHeight() {
	return axisHeight;
}
int Slider::getSliderWidth() {
	return sliderWidth;
}
int Slider::getSliderHeight() {
	return sliderHeight;
}

void Slider::step() {
	set_scale(b2Vec2(sliderWidth, sliderHeight));
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
	draw(*get_draw()->get_window());
}
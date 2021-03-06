#pragma once
#include "Menu_Object.h"
class Slider : public Menu_Object {
private:
	sf::RectangleShape slider;
	sf::RectangleShape axis;
	sf::Font font;
	sf::Text text;
	int minValue;
	int maxValue;
	int xCord;
	int yCord;
	int axisWidth;
	int axisHeight;
	int sliderWidth;
	int sliderHeight;
	float sliderValue;
	bool font_active = 0;
	bool slider_active = 0;
	sf::Vector2f mouse_pos_;
	sf::Text returnText(int x, int y, std::string z, int fontSize);
	void logic(sf::RenderWindow& window);
	void draw(sf::RenderWindow& window);
public:
	Slider();
	void init();

	// Set methods
	void create(int min, int max);
	void setSliderValue(float newValue);
	void setSliderPercentValue(float newPercentValue);
	void setAxisWidth(int newAxisWidth);
	void setAxisHeight(int newAxisHeight);
	void setSliderWidth(int newSliderWidth);
	void setSliderHeight(int newSliderHeight);

	// Get methods
	float getSliderValue();
	int getAxisWidth();
	int getAxisHeight();
	int getSliderWidth();
	int getSliderHeight();

	void step();
};


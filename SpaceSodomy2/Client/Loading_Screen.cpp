#include "Loading_Screen.h"

Loading_Screen::Loading_Screen(Draw* draw_, tgui::Gui &gui) {
	draw = draw_;
	loading_stage->getRenderer()->setTextColor(sf::Color::White);
	loading_stage->setPosition("25%", "55%");
	loading_stage->setSize("50%", "10%");
	loading_stage->setTextSize(15);
	loading_stage->setVisible(1);
	progress_bar->getRenderer()->setBackgroundColor(sf::Color(140, 140, 140, 255));
	progress_bar->getRenderer()->setFillColor(sf::Color(200, 200, 200, 255));
	loading_stage->setSize("50%", "10%");
	progress_bar->setPosition("25%", "50%");
	progress_bar->setMaximum(100);
	progress_bar->setValue(0);
	progress_bar->setVisible(1);
	gui.add(loading_stage);
	gui.add(progress_bar);
	_gui = &gui;
}

void Loading_Screen::step(int val, std::string stage) {
	draw->fill_rect({ 0, 0 }, aux::to_b2Vec2(sf::Vector2f(draw->get_window()->getSize())),
		sf::Color(0, 0, 0, 255), 0);
	progress_bar->setValue(val);
	loading_stage->setText(stage);
	draw->text("Loading...", "font", { 0, -50 }, 50, sf::Color::White);
	_gui->draw();
	draw->display();
}

void Loading_Screen::close() {
	_gui->remove(loading_stage);
	_gui->remove(progress_bar);
}

#include "pch.h"
#include "Dropdown_list.h"

void Dropdown_list::add_button(int id, std::string texture_name, float scale_x, float scale_y, sf::Color color, b2Vec2* mouse_pos) {
	buttons.push_back(new Button);
	buttons.back()->set_id(id);
	buttons.back()->set_texture_name(texture_name);
	buttons.back()->set_scale(b2Vec2(scale_x, scale_y));
	buttons.back()->set_color(color);
	buttons.back()->set_mouse_pos(mouse_pos);
	b2Vec2 indent = { 0, 0 };
	for (int i = 0; i < buttons.size() - 1; i++) {
		indent.y += buttons[i]->get_scale().y;
	}
	indent.x += buttons.back()->get_scale().x / 2.0;
	indent.x -= get_pos().x / 2.0;
	indent.y += buttons.back()->get_scale().y / 2.0;
	indent.y -= get_pos().y / 2.0;
	buttons.back()->set_pos(get_pos() + indent);
}

void Dropdown_list::set_events(std::queue<int>* events_) {
	events = events_;
}

void Dropdown_list::step() {
	for (auto button : buttons) {
		button->step();
		if (*get_clicked() && button->get_active())
			events->push(button->get_id());
	}
}
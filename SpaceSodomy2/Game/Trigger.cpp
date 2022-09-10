#include "pch.h"
#include "Trigger.h"

Trigger::Trigger(b2Vec2 _center, float _radius, const GameReadable& _) : game(_) {
	center = _center;
	radius = _radius;
}

void Trigger::add_id(int id) {
	ids.insert(id);
}

void Trigger::remove_id(int id) {
	ids.erase(ids.find(id));
}

void Trigger::set_function(std::function<void(Ship*)> _trigger_action) {
	trigger_action = _trigger_action;
}

void Trigger::step() {
	for (auto i = ids.begin(); i != ids.end(); i++) {
		for (auto it = game.ships.begin(); it != game.ships.end(); it++) {
			auto ship = *it;
			if (ship->get_id() == *i) {
				if ((ship->get_body()->GetPosition() - center).Length() < radius) {
					trigger_action(ship);
				}
				else {
					std::cout << (ship->get_body()->GetPosition() - center).Length() << "\n";
				}
			}
		}
	}
}
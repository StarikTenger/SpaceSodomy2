#pragma once
#include <set>
#include <box2d/box2d.h>
#include <functional>
#include <Game/GameReadable.h>
#include <Game/Ship.h>
class Trigger {
private:
	b2Vec2 center;
	float radius;
	std::set<int> ids;
	std::function<void(Ship *)> trigger_action;
	const GameReadable& game;

public:
	Trigger(b2Vec2 _center, float _radius, const GameReadable&);

	void set_function(std::function<void(Ship*)> _trigger_action);
	void add_id(int id);
	void remove_id(int id);
	void step();
};


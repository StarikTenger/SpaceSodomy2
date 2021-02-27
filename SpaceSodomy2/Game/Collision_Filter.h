#pragma once
#include <box2d/box2d.h>
#include <map>
#include <vector>

class Collision_Filter : public b2ContactFilter{
private:
	std::map<b2Body*, int> collision_table;
	int get_type(b2Fixture*);
	std::vector<std::pair<int, int>> collide_pairs{
		{STANDART, STANDART},
		{STANDART, WALL},
		{PROJECTILE, WALL}
	};
public:
	enum Collision_Type {
		STANDART,
		WALL,
		PROJECTILE
	};

	Collision_Filter();
	void add_body(b2Body*, int);
	bool ShouldCollide(b2Fixture*, b2Fixture*) override;
};


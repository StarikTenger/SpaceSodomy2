#pragma once
#include <box2d/box2d.h>
#include <map>
#include <vector>

class Collision_Filter : public b2ContactFilter{
private:
	struct Collision_Info {
		int type = STANDART;
		int id = 0;
	};

	std::map<b2Body*, Collision_Info> collision_table;
	int get_type(b2Fixture*);
	int get_id(b2Fixture*);

	// IDs: 1 equal, -1 not equal
	std::vector<std::vector<int>> collide_pairs{
		{STANDART, STANDART, 0},
		{STANDART, WALL, 0},
		{PROJECTILE, WALL, 0},
		{PROJECTILE, STANDART, -1}
	};
public:
	enum Collision_Type {
		STANDART,
		WALL,
		PROJECTILE
	};

	Collision_Filter();
	void add_body(b2Body*, int type = STANDART, int id = 0);
	bool ShouldCollide(b2Fixture*, b2Fixture*) override;
};


#pragma once
#include <box2d/box2d.h>
#include <map>
#include <vector>

class Collision_Filter {
private:
	struct Collision_Info {
		int type = STANDART;
		int id = 0;
	};

	std::map<b2Body*, Collision_Info> collision_table;
	int get_id(b2Fixture*);

	// IDs: 1 equal, -1 not equal
	std::vector<std::vector<int>> collide_pairs{
		{STANDART, STANDART, 0},
		{STANDART, WALL, 0},
		{PROJECTILE, WALL, 0},
	};
public:
	enum Collision_Type {
		STANDART,
		WALL,
		GHOST,
		PROJECTILE,
	};

	Collision_Filter();
	void add_body(b2Body*, int type = STANDART, int id = 0);
	void delete_body(b2Body*);
	int get_type(b2Fixture*);
	bool ShouldCollide(b2Fixture*, b2Fixture*);
	void change_body(b2Body*, Collision_Type);
};


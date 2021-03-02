#pragma once
#include <box2d/box2d.h>
#include <set>

class Contact_Table {
private:
	std::set<std::pair<b2Body*, b2Body*>> table;

public:
	Contact_Table();
	void reset();
	void add(b2Body*, b2Body*);
	bool check(b2Body*, b2Body*);
};


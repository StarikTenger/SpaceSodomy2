#include "pch.h"
#include "Contact_Table.h"

Contact_Table::Contact_Table() {
}

void Contact_Table::reset() {
	table.clear();
}

void Contact_Table::add(b2Body* body_a, b2Body* body_b) {
	table.insert({ body_a, body_b });
}

bool Contact_Table::check(b2Body* body_a, b2Body* body_b) {
	return table.find({ body_a, body_b }) != table.end() || 
		table.find({ body_b, body_a }) != table.end();
}
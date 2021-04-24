#include "pch.h"
#include "Check_Box.h"

Check_Box::Check_Box() {
	set_texture_name("CheckBoxInactive");
}

void Check_Box::set_state(bool state_) {
	state = state_;
	if (state == false)
		set_texture_name("CheckBoxInactive");
	else
		set_texture_name("CheckBoxActive");
}
void Check_Box::change_state() {
	state = !state;
	if (state == false)
		set_texture_name("CheckBoxInactive");
	else
		set_texture_name("CheckBoxActive");
}

bool Check_Box::get_state() {
	return state;
}

void Check_Box::step() {
	if (*get_clicked() && get_active()) {
		change_state();
	}

	primitive_step();
}
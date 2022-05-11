#include "ControlMapEditor.h"

int main() {
	// Init random seed
	aux::random_seed(aux::get_milli_count());

	// Init control
	ControlMapEditor control;

	control.run();
}
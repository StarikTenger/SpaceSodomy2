#include "Control.h"

int main() {
	// Init random seed
	aux::random_seed(aux::get_milli_count());

	// Init control
	Control control;

	control.run();
}
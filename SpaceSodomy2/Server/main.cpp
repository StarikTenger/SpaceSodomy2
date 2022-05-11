#include <iostream>
#include <AuxLib/AuxLib.h>
#include "Control.h"

int main() {
	aux::random_seed(aux::get_milli_count());
	ControlServer control;
	control.run();
}
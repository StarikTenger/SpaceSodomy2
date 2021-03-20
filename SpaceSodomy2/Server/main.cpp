#include <iostream>
#include <AuxLib/AuxLib.h>
#include "Control.h"

int main() {
	Control control;
	aux::random_seed(aux::get_milli_count());
	while (1) {  
		control.step();
	}
}
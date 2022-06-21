#include <iostream>
#include <AuxLib/AuxLib.h>
#include "Control.h"
#include "ServerConfigCreator.h"

int main() {
	// Create configs
	ServerConfigCreator config_creator;
	config_creator.run();

	aux::random_seed(aux::get_milli_count());
	Control control;
	while (1) {  
		control.step();
	}
}
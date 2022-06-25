#include <iostream>
#include <AuxLib/AuxLib.h>
#include "Control.h"
#include "ServerConfigCreator.h"

int main(int argc, char* argv[]) {
	std::cout << "Have " << argc << " arguments:" << std::endl;
	for (int i = 0; i < argc; ++i) {
		std::cout << argv[i] << std::endl;
	}

	// Create configs
	ServerConfigCreator config_creator;
	config_creator.run();

	aux::random_seed(aux::get_milli_count());
	Control control;

	// Set idle timeout
	if (argc > 1) {
		std::cout << "idle timeout set " << atoi(argv[1]) << "\n";
		control.set_idle_timeout(atoi(argv[1]));
	}

	while (control.get_is_running()) {
		DEBUG_PRINT("nigger")
		control.step();
	}

	return 0;
}
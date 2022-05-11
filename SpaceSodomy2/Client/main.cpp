#include "Control.h"
#include "ClientConfigCreator.h"

int main() {
	// Create configs
	ClientConfigCreator config_creator;
	config_creator.run();

	// Init random seed
	aux::random_seed(aux::get_milli_count());

	// Init control
	Control control;		
	control.load_config("client_config.conf");
	control.run();
	
}
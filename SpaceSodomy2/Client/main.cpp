#include "Control.h"

int main() {
	Control control;
	control.load_config("client_config.conf");
	aux::random_seed(aux::get_milli_count());
	while (control.get_is_running()) {
		control.step();
	}
}
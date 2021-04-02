#include "Control.h"

int main() {
	aux::random_seed(aux::get_milli_count());
	Control control;
	control.load_config("client_config.conf");
	while (control.get_is_running()) {
		control.step();
	}
}
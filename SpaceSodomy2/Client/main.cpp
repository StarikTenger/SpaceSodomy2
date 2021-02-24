#include "Control.h"

int main() {
	Control control;
	control.load_config("client_config.conf");
	while (control.get_is_running()) {
		control.step();
	}
}
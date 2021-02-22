#include "Control.h"

int main() {
	Control control;
	while (control.get_is_running()) {
		control.step();
	}
}
#include "Draw/Draw.h"

int main() {
	Draw draw;
	draw.create_window(600, 600, "TEST");
	draw.fill_circle({ 10, 10 }, 20, {255, 255, 255});
	draw.display();
	for (;;);
}
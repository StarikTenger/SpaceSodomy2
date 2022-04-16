#include "pch.h"
#include "ShipBrain.h"


ShipBrain::ShipBrain(CommandModule& a, const GameReadable& b, Ship* ptr) : iBrain(a, b), ship(ptr) {}
void ShipBrain::set_ship(Ship* _) { ship = _; }

#include "pch.h"
#include "ShipContinuity.h"

int ShipContinuity::get_deaths() const { return deaths; }
bool ShipContinuity::is_alive() const { return ship; }
void ShipContinuity::set_new_ship(Ship* _) { ship = _; }
Ship* ShipContinuity::release_ship() {
    auto res = ship;
    ship = nullptr;
    brain->set_ship(nullptr);
    return res;
}

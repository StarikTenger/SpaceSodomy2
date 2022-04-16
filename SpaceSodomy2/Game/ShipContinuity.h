#pragma once
#include "Ship.h"
#include "ShipBrain.h"

// TODO: change name
class ShipContinuity {
public:
    int get_deaths() const;
    bool is_alive() const;
    void set_new_ship(Ship*);
    Ship* release_ship();
    ShipBrain* brain;
private:
    int deaths = 0;
    Ship* ship = nullptr;

};
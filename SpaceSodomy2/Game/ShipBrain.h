#pragma once
#include "Brain.h"
#include "Ship.h"

class ShipBrain : public iBrain {
public:
    ShipBrain(CommandModule&, const GameReadable&,  Ship* = nullptr);
    void set_ship(Ship*); // Assert this->ship == nullptr;
protected:
    Ship* ship;
};



class PlayerBrain : public ShipBrain {
public:
    void compute_action() final {};
};
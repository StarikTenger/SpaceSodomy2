#pragma once
#include "Brain.h"

class ShipBrain : public iBrain {
protected:
    int id;
public:
    ShipBrain(CommandModule&, const GameReadable&, int = 0);
    void set_new_id(int);
};

class NetworkShipBrain : public ShipBrain {
public:
    NetworkShipBrain(CommandModule&, const GameReadable&, int = 0);

    // Do nothing
    void compute_action() final {};
};

class AiShipBrain : public ShipBrain {
public:
    AiShipBrain(CommandModule&, const GameReadable&, int = 0);
    void compute_action() final;
};
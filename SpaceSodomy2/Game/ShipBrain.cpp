#include "pch.h"
#include "ShipBrain.h"
#include "Ship.cpp"


ShipBrain::ShipBrain(CommandModule& _1, const GameReadable& _2, int _3) : iBrain(_1, _2), id(_3) {};
void ShipBrain::set_new_id(int _) { id = _; }


NetworkShipBrain::NetworkShipBrain(CommandModule& _1, const GameReadable& _2, int _3) : ShipBrain(_1, _2, _3) {};
AiShipBrain::AiShipBrain(CommandModule& _1, const GameReadable& _2, int _3) : ShipBrain(_1, _2, _3) {};


void AiShipBrain::compute_action() {
    Ship* my_ship = nullptr;
    for (auto ship : environment.ships) {
        if (ship->get_id() == id) {
            my_ship = ship;
        }
    }
    if (my_ship) {
        std::cout << "IT'S ALIIIIIIIIIIIIIIIIIVE!\n";
        /*
        * AI code goes here
        */
    }
    else {
        command_module.set_command(CommandModule::RESPAWN, 1);
    }
}
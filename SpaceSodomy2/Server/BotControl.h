#pragma once
#include "Game/ShipBrain.h"

class BotControl {
private:
    CommandModule cmd_module = CommandModule();
    ShipBrain* brain = nullptr;
    std::string name;
    int id = aux::random_int(0, 1000000000);
    int token = aux::random_int(0, 1000000000);
    std::string quasi_IP_addr = "*" + std::to_string(aux::random_int(0, 1000000));
public:
    BotControl(std::string bot_name, ShipBrain::Type type, const GameReadable&);
    BotControl(BotControl&&) = delete;
    BotControl& operator=(BotControl&&) = delete;
    ~BotControl();
    void set_equip(std::string bot_name, ShipBrain::Equip equipment);


    // The bot is lazy : if a message is not needed, he does not compute action
    std::string get_message();
};
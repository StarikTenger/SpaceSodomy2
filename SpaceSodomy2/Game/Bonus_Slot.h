#pragma once
#include "Bonus.h"
#include "Bonus_Manager.h"

class Bonus_Slot {
protected:
    Bonus_Prototype* current_bonus = nullptr;
    Effects* effects = nullptr;
public:
    void add_bonus(Bonus_Prototype*);
    bool activate();
    void set_effects(Effects*);
};


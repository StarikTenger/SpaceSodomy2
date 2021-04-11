#pragma once
#include "Bonus.h"

class Bonus_Slot {
protected:
    Bonus_Def* current_bonus;
public:
    void add_bonus(Bonus_Def*);
    void activate();
};


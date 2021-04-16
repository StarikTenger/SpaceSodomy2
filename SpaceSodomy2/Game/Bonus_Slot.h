#pragma once
#include "Bonus.h"
#include "Bonus_Manager.h"

class Bonus_Slot {
protected:
    Bonus::Types current_bonus = Bonus::COUNT;
    Effects* effects = nullptr;
    Bonus_Manager* bonus_manager;
public:
    void add_bonus(Bonus::Types type);
    Bonus::Types get_current_bonus();
    bool activate();
    void set_effects(Effects*);
    void set_bonus_manager(Bonus_Manager*);
    void set_current_bonus(int val);
};


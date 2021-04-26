#pragma once
#include "Bonus.h"
#include "Bonus_Manager.h"
#include "Active_Module.h"

class Bonus_Slot : public Active_Module {
protected:
    Bonus::Types current_bonus = Bonus::COUNT;
    Bonus_Manager* bonus_manager = nullptr;

public:
    Bonus_Slot();
    Bonus::Types get_current_bonus();
    void activate() override;
    void activate_side_effects() override;
    void set_bonus_manager(Bonus_Manager*);
    void set_current_bonus(int val);
    void add_bonus(Bonus::Types type);
};


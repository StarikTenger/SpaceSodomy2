#pragma once
#include "Bonus.h"
#include "Event.h"
#include "BonusManager.h"
#include "ActiveModule.h"

class BonusSlot : public ActiveModule {
protected:
    Bonus::Types current_bonus = Bonus::COUNT;
    BonusManager* bonus_manager = nullptr;

public:
    BonusSlot();
    Bonus::Types get_current_bonus();
    void activate() override;
    void set_bonus_manager(BonusManager*);
    void set_current_bonus(int val);
    void add_bonus(Bonus::Types type);
};


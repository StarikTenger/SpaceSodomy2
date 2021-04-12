#include "pch.h"
#include "Bonus_Slot.h"


void Bonus_Slot::add_bonus(Bonus_Prototype* val) {
    if (!val) {
        return;
    }
    if (val->is_instant) {
        effects->update(&val->effects_prototype);
    }
    else {
        current_bonus = val;
    }
}
bool Bonus_Slot::activate() {
    if (!current_bonus) {
        return false;
    }
    effects->update(&current_bonus->effects_prototype);
    current_bonus = nullptr;
    return true;
}
void Bonus_Slot::set_effects(Effects* val) {
    effects = val;
}
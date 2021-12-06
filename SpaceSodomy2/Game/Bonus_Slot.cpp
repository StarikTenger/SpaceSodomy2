#include "pch.h"
#include "BonusSlot.h"

BonusSlot::BonusSlot() {
    bind = CommandModule::BONUS_ACTIVATION;
    recharge_time = 0;
    stamina_cost = 0;
}
void BonusSlot::add_bonus(Bonus::Types val) {
    if (val >= Bonus::COUNT) {
        return;
    }
    if (bonus_manager->get_prototype(val)->is_instant) {
        effects->update(&bonus_manager->get_prototype(val)->effects_prototype);
    }
    else {
        current_bonus = val;
    }
}

Bonus::Types BonusSlot::get_current_bonus() {
    return current_bonus;
}
void BonusSlot::activate() {
    if (current_bonus == Bonus::LASER) {
        event_manager->create_event(EventDef(Event::LASER, get_body()));
    }
    activate_default_side_effects();

    if (current_bonus >= Bonus::COUNT) {
        return;
    }
    effects->update(&(bonus_manager->get_prototype(current_bonus)->effects_prototype));
    current_bonus = Bonus::COUNT;
}

void BonusSlot::set_bonus_manager(BonusManager* val) {
    bonus_manager = val;
}

void BonusSlot::set_current_bonus(int val) {
    current_bonus = (Bonus::Types)val;
}

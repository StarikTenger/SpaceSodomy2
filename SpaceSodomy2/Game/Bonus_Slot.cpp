#include "pch.h"
#include "Bonus_Slot.h"

Bonus_Slot::Bonus_Slot() {
    bind = Command_Module::BONUS_ACTIVATION;
    recharge_time = 0;
    stamina_cost = 0;
}
void Bonus_Slot::add_bonus(Bonus::Types val) {
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

Bonus::Types Bonus_Slot::get_current_bonus() {
    return current_bonus;
}
void Bonus_Slot::activate() {
    if (current_bonus == Bonus::LASER) {
        event_manager->create_event(Event_Def("laser", get_body()));
    }
    activate_default_side_effects();

    if (current_bonus >= Bonus::COUNT) {
        return;
    }
    effects->update(&(bonus_manager->get_prototype(current_bonus)->effects_prototype));
    current_bonus = Bonus::COUNT;
}

void Bonus_Slot::set_bonus_manager(Bonus_Manager* val) {
    bonus_manager = val;
}

void Bonus_Slot::set_current_bonus(int val) {
    current_bonus = (Bonus::Types)val;
}

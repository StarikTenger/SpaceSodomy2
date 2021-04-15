#include "pch.h"
#include "Effects.h"


Effects::Effect::Effect() {
    duration.set_change_vel(-1);
    type = Algebraic_Type::ADDITIVE;
    strength = 0;
}
Effects::Effect::Effect(Algebraic_Type _type) {
    type = _type;
    duration.set_change_vel(-1);
    strength = 0;
}
Effects::Effect::Effect(Counter counter, Algebraic_Type _type) {
    type = _type;
    duration = counter;
    duration.set_change_vel(-1);
    strength = 0;
}
Effects::Effect::Effect(float time, Algebraic_Type _type) {
    type = _type;
    duration = Counter(time);
    duration.set_change_vel(-1);
    strength = 0;
}

Effects::Algebraic_Type Effects::Effect::get_type() {
    return type;
}
Counter* Effects::Effect::get_counter() {
    return &duration;
}
float Effects::Effect::get_strength() {
    return strength;
}


void Effects::Effect::set_type(Algebraic_Type _type) {
    type = _type;
}


void Effects::Effect::set_counter(Counter counter) {
    duration = counter;
}

void Effects::Effect::step(float dt) {
    duration.step(dt);
}

void Effects::Effect::set_strength(float val) {
    strength = val;
}

Effects::Effect& Effects::Effect::operator+=(Effect other) {
    if (type == Algebraic_Type::ANNULATOR) {
        return *this;
    }
    if (duration.get() < 0) {
        duration.set(0);
    }

    ////TO REMOVE LATER: FOR DEBUG
    //if (type != other.type) {
    //    std::cout << "Error adding effects: Algebraic types are ";
    //    switch (type) {
    //    case Algebraic_Type::MAXIMAL:
    //        std::cout << "MAXIMAL ";
    //        break;
    //    case Algebraic_Type::ADDITIVE:
    //        std::cout << "ADDITIVE ";
    //        break;
    //    case Algebraic_Type::ANNULATOR:
    //        std::cout << "ANNULATOR ";
    //        break;
    //    }
    //    std::cout << "and ";
    //    switch (type) {
    //    case Algebraic_Type::MAXIMAL:
    //        std::cout << "MAXIMAL ";
    //        break;
    //    case Algebraic_Type::ADDITIVE:
    //        std::cout << "ADDITIVE ";
    //        break;
    //    case Algebraic_Type::ANNULATOR:
    //        std::cout << "ANNULATOR ";
    //        break;
    //    }
    //    std::cout << "\n";
    //}


    if (other.duration.get() < b2_epsilon) {
        return *this;
    }
    switch (type) {
    case Algebraic_Type::MAXIMAL:
        duration.set(std::max(duration.get(), other.duration.get()));
        break;
    case Algebraic_Type::ADDITIVE:
        duration.modify(other.duration.get());
        break;
    }
    return *this;

}
Effects::Effect Effects::Effect::operator+(Effect effect) {
    auto left = *this;
    left += effect;
    return left;
}



Effects::Effects() : effects(Effects::Types::COUNT) {
}

Effects::Effects(Effects_Prototype* def) : effects(Effects::Types::COUNT) {
    for (int i = 0; i < def->effects.size(); i++) {
        effects[i] = (def->effects[i]);
    }
}
Effects::Effect* Effects::get_effect(Types type) {
    return &effects[type];
}
void Effects::set_effect(Effect* eff, Types type) {
    effects[type] = *eff;
}

void Effects::step(float dt) {
    for (int i = 0; i < Effects::Types::COUNT; i++) {
        effects[i].step(dt);
    }
}

void Effects::update(Effects_Prototype* _effects) {
    if (!_effects) {
        return;
    }
    for (int i = 0; i < Types::COUNT; i++) {
        effects[i] += _effects->effects[i];
    }
}


Effects_Prototype::Effects_Prototype() {
    for (int i = 0; i < Effects::Types::COUNT; i++) {
        Effects::Effect eff;
        effects.push_back(eff);
    }
}

Effects_Prototype::Effects_Prototype(Effects::Algebraic_Type type) {
    for (int i = 0; i < Effects::Types::COUNT; i++) {
        effects.push_back(Effects::Effect(type));
    }
}


Effects::Types Effects::get_effect_type(std::string effect_name) {
    if (effect_name == "INSTANT_HP")
        return Effects::Types::INSTANT_HP;
    if (effect_name == "INSTANT_STAMINA")
        return Effects::Types::INSTANT_STAMINA;
    if (effect_name == "LASER")
        return Effects::Types::LASER;
    if (effect_name == "LASER_BURN")
        return Effects::Types::LASER_BURN;
    if (effect_name == "CHARGE")
        return Effects::Types::CHARGE;
    if (effect_name == "BERSERK")
        return Effects::Types::BERSERK;
    if (effect_name == "IMMORTALITY")
        return Effects::Types::IMMORTALITY;
    else {
        std::cerr << "incorrect effect type : " << effect_name << '\n';
        return Effects::Types::INSTANT_HP;
    }
}

#include "pch.h"
#include "Effects.h"

Effects::Effect::Effect() {
    type = Algebraic_Type::ANNULATOR;
}
Effects::Effect::Effect(Algebraic_Type _type) {
    type = _type;
}
Effects::Effect::Effect(Counter counter, Algebraic_Type _type) {
    type = _type;
    duration = counter;
}
Effects::Effect::Effect(float time, Algebraic_Type _type) {
    type = _type;
    duration = Counter(time);
}

Effects::Algebraic_Type Effects::Effect::get_type() {
    return type;
}
Counter* Effects::Effect::get_counter() {
    return &duration;
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

Effects::Effect& Effects::Effect::operator+=(Effect other) {
    if (type == Algebraic_Type::ANNULATOR) {
        return *this;
    }
    //TO REMOVE LATER: FOR DEBUG
    if (type != other.type) {
        std::cout << "Error adding effects: Algebraic types are ";
        switch (type) {
        case Algebraic_Type::MAXIMAL:
            std::cout << "MAXIMAL ";
            break;
        case Algebraic_Type::ADDITIVE:
            std::cout << "ADDITIVE ";
            break;
        case Algebraic_Type::ANNULATOR:
            std::cout << "ANNULATOR ";
            break;
        }
        std::cout << "and ";
        switch (type) {
        case Algebraic_Type::MAXIMAL:
            std::cout << "MAXIMAL ";
            break;
        case Algebraic_Type::ADDITIVE:
            std::cout << "ADDITIVE ";
            break;
        case Algebraic_Type::ANNULATOR:
            std::cout << "ANNULATOR ";
            break;
        }
        std::cout << "\n";
    }


    if (other.duration.get() < b2_epsilon) {
        return *this;
    }
    set_id(other.get_id());
    switch (type) {
    case Algebraic_Type::MAXIMAL:
        duration.set(std::max(duration.get(), other.duration.get()));
        break;
    case Algebraic_Type::ADDITIVE:
        duration.modify(other.duration.get());
        break;
        return *this;
    }
}
Effects::Effect Effects::Effect::operator+(Effect effect) {
    auto left = *this;
    left += effect;
    return left;
}



Effects::Effects() : effects(Effects::Effect_Type::COUNT) {
}
Effects::Effects(Effects_Def def, int id) {
    effects = def.effects;
    for (auto i : effects) {
        i.set_id(id);
    }
}
Effects::Effect* Effects::get_effect(Effect_Type type) {
    return &effects[type];
}
void Effects::set_effect(Effect* eff, Effect_Type type) {
    effects[type] = *eff;
}

void Effects::step(float dt) {
    for (auto i : effects) {
        i.step(dt);
    }
}

Effects& Effects::operator+=(Effects other) {
    for (int i = 0; i < effects.size(); i++) {
        effects[i] += other.effects[i];
    }
    return *this;
}
Effects& Effects::operator+=(Effects_Def other) {
    for (int i = 0; i < effects.size(); i++) {
        effects[i] += other.effects[i];
    }
    return *this;
}
Effects Effects::operator+(Effects effect) {
    auto left = *this;
    left += effect;
    return left;
}
Effects Effects::operator+(Effects_Def effect) {
    auto left = *this;
    left += effect;
    return left;
}


void Effects::update(Effects_Def _effects, int id) {
    Effects eff(_effects, id);
    *this += eff;
}


Effects_Def::Effects_Def() {
    for (int i = 0; i < Effects::Effect_Type::COUNT; i++) {
        effects.push_back(Effects::Effect());
    }
}

Effects_Def::Effects_Def(Effects::Algebraic_Type type) {
    for (int i = 0; i < Effects::Effect_Type::COUNT; i++) {
        effects.push_back(Effects::Effect(type));
    }
}

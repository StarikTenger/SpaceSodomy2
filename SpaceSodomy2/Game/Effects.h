#pragma once
#include <deque>
#include <AuxLib/AuxLib.h>
#include "Counter.h"

struct Effects_Def;

class Effects {
public:
    enum Types {
        LASER_BURN,
        BERSERK,
        CHARGE,

        COUNT
    };
    enum Algebraic_Type {
        ANNULATOR,         // multiplicative zero
        ADDITIVE,
        MAXIMAL
    };
    class Effect {
    private:
        Counter duration;
        Algebraic_Type type;
        float strength = 0;
    public:
        Effect();
        Effect(Algebraic_Type);
        Effect(Counter, Algebraic_Type = Algebraic_Type::ADDITIVE);
        Effect(float, Algebraic_Type = Algebraic_Type::ADDITIVE);

        Algebraic_Type get_type();
        Counter* get_counter();

        void set_type(Algebraic_Type);
        void set_counter(Counter);


        void step(float dt);

        Effect& operator+=(Effect other);
        Effect operator+(Effect effect);
    };

private:
    std::deque<Effect> effects;

public:

    Effects();
    Effects(Effects_Def*);

    Effect* get_effect(Types type);
    void set_effect(Effect*, Types type);

    void step(float dt);

    void update(Effects_Def* effects);

};

struct Effects_Def {
public:
    std::deque<Effects::Effect> effects;

    Effects_Def();
    Effects_Def(Effects::Algebraic_Type);
};
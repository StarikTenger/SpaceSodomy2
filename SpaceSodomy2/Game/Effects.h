#pragma once
#include <deque>
#include <AuxLib/AuxLib.h>
#include "Counter.h"

struct Effects_Def;

class Effects {
public:
    enum Effect_Type {
        LASER_BURN,
        COUNT
    };
    enum Algebraic_Type {
        ANNULATOR,         // multiplicative zero
        ADDITIVE,
        MAXIMAL
    };
    class Effect : public iId {
    private:
        Counter duration;
        Algebraic_Type type;
    public:
        // Creates an annulator effect
        Effect();
        Effect(Algebraic_Type);
        Effect(Counter, Algebraic_Type = Algebraic_Type::MAXIMAL);
        Effect(float, Algebraic_Type = Algebraic_Type::MAXIMAL);

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

    Effects& operator+=(Effects other);
    Effects& operator+=(Effects_Def other);
    Effects operator+(Effects effect);
    Effects operator+(Effects_Def effect);

public:

    Effects();
    Effects(Effects_Def, int id);

    Effect* get_effect(Effect_Type type);
    void set_effect(Effect*, Effect_Type type);

    void step(float dt);

    void update(Effects_Def effects, int id);

};

struct Effects_Def {
public:
    std::deque<Effects::Effect> effects;

    Effects_Def();
    Effects_Def(Effects::Algebraic_Type);
};
#pragma once
#include <vector>
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
        MAXIMAL,
        CONSTANT
    };
    class Effect : public iId {
    public:
        Counter duration;
        Algebraic_Type type;
   
        // Creates an annulator effect
        Effect();
        Effect(Algebraic_Type);
        Effect(Counter, Algebraic_Type = Algebraic_Type::MAXIMAL);
        Effect(float, Algebraic_Type = Algebraic_Type::MAXIMAL);

        void step(float dt);

        Effect& operator+=(Effect other);
        Effect operator+(Effect effect);
    };

public:
    std::vector<Effect> effects;

    Effects();
    Effects(Effects_Def, int id);

    Counter* get_effect(Effect_Type type);
    float get_effect_duration(Effect_Type type);
    void set_effect(Counter*, Effect_Type type);
    void set_effect(float, Effect_Type type);

    void step(float dt);

    Effects& operator+=(Effects other);
    Effects& operator+=(Effects_Def other);
    Effects operator+(Effects effect);
    Effects operator+(Effects_Def effect);

};

struct Effects_Def {
public:
    std::vector<Effects::Effect> effects;

    Effects_Def();
};
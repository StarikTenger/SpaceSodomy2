#pragma once
#include <deque>
#include <AuxLib/AuxLib.h>
#include "Counter.h"
#include <map>

struct Effects_Prototype;

class Effects {
public:
    enum Types {
        INSTANT_HP,
        INSTANT_STAMINA,
        LASER,
        WALL_BURN,
        CHARGE,
        BERSERK,
        IMMORTALITY,
        INVISIBILITY,
        COUNT
    };
    static Effects::Types get_effect_type(std::string effect_name);
    enum Algebraic_Type {
        ANNULATOR,         // multiplicative zero
        ADDITIVE,
        MAXIMAL,
        NO_OVERLAY
    };
    class Effect {
    private:
        Counter duration;
        float strength;
        std::map<std::string, float> params;
        Algebraic_Type type;
    public:
        Effect();
        Effect(Algebraic_Type);
        Effect(Counter, Algebraic_Type = Algebraic_Type::ADDITIVE);
        Effect(float, Algebraic_Type = Algebraic_Type::ADDITIVE);

        Algebraic_Type get_type();
        Counter* get_counter();
        float get_param(std::string);

        void set_type(Algebraic_Type);
        void set_counter(Counter);
        void set_param(std::string, float);


        void step(float dt);

        Effect& operator+=(Effect other);
        Effect operator+(Effect effect);
    };

private:
    std::deque<Effect> effects;

public:

    Effects();
    Effects(Effects_Prototype*);

    std::vector<int> get_mask();


    Effect* get_effect(Types type);
    void set_effect(Effect*, Types type);

    void step(float dt);

    void update(Effects_Prototype* effects);
    void update(Effects::Types, float val);

};

struct Effects_Prototype {
public:
    std::deque<Effects::Effect> effects;

    Effects_Prototype();
    Effects_Prototype(Effects::Algebraic_Type);
};
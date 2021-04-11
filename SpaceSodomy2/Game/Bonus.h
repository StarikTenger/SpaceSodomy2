#pragma once
#include <box2d/box2d.h>
#include "Effects.h"

struct Bonus_Def;

class Bonus {
private:
    b2Body* body = nullptr;
    Bonus_Def* bonus_def = nullptr;
public:
    enum Types {
        INSTANT_HP,
        INSTANT_STAMINA,
        LASER,
        CHARGE,
        BERSERK,
        IMMORTALITY,
        COUNT
    };
    void set_bonus_def(Bonus_Def* _bonus_def);
    void set_body(b2Body*);
    Bonus_Def* get_bonus_def();
    b2Body* get_body();

    Bonus() = default;
};

struct Bonus_Def {
    bool is_instant = 0;
    Effects_Def effects_def;
};

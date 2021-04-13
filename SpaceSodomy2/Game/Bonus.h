#pragma once
#include <box2d/box2d.h>
#include "Effects.h"
#include "iId.h"

struct Bonus_Prototype;

class Bonus : public iId {
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
    static Bonus::Types get_bonus_type(std::string name);
private:
    b2Body* body = nullptr;
    Bonus_Prototype* bonus_prototype = nullptr;
public:
    void set_bonus_prototype(Bonus_Prototype* _bonus_prototype);
    void set_body(b2Body*);
    Bonus_Prototype* get_bonus_prototype();
    Bonus::Types get_type();
    b2Body* get_body();
    Bonus() = default;
};

struct Bonus_Prototype {
    bool is_instant = 0;
    Bonus::Types type = Bonus::Types::INSTANT_HP;
    float radius = 0.2;
    Effects_Prototype effects_prototype;
};

struct Bonus_Def : iId {
    Bonus_Prototype* bonus = nullptr;
    b2Vec2 pos;
};
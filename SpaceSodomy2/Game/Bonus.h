#pragma once
#include <box2d/box2d.h>
#include "Effects.h"
#include "iId.h"

struct Bonus_Prototype;

class Bonus : public iId {
public:
    enum Types {
        INSTANT_HP,
        INSTANT_ENERGY,
        LASER,
        CHARGE,
        BERSERK,
        IMMORTALITY,
        COUNT
    };
    static Bonus::Types get_bonus_type(std::string name);
    static std::string get_bonus_name(Bonus::Types type);
private:
    b2Body* body = nullptr;
    Bonus::Types type;
public:
    void set_type(Bonus::Types type);
    void set_body(b2Body*);
    Bonus::Types get_type();
    b2Body* get_body();
    Bonus() = default;
};

struct Bonus_Prototype {
    bool is_instant = 0;
    float radius = 0.2;
    Bonus::Types type = Bonus::Types::INSTANT_HP;
    Effects_Prototype effects_prototype;
};

struct Bonus_Def : public iId {
    Bonus::Types type;
    b2Vec2 pos;
};
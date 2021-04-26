#pragma once
#include "pch.h"
#include "Active_Module.h"

struct Module_Prototype;

class Module : public Active_Module {
public:
    Module(Module_Prototype*);
    enum Type {
        SHOTGUN,
        ROCKET,
        IMMORTALITY,
        INVISIBILITY,
        BLINK,
        DASH,
        FORCE,
        COUNT
    };
private:
    Type type = COUNT;
public:
    Type get_type();
    void set_bind(bool is_left);
};

class Shotgun_Module : public Module {
public:
    void activate() override;
    void activate_side_effects() override;
};

struct Module_Prototype {
    Effects_Prototype effects_prototype;
    float stamina_cost;
    float energy_cost;
    float cooldown;
    Module::Type type;
};
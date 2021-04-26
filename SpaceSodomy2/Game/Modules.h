#pragma once
#include "pch.h"
#include "Active_Module.h"
#include "Projectile_Manager.h"

struct Module_Prototype;

class Shotgun_Module;
class Immortality_Module;
class Invisibility_Module;
class Dash_Module;

class Module : public Active_Module {
public:
    enum Type {
        SHOTGUN,
        IMMORTALITY,
        INVISIBILITY,
        DASH,
        COUNT
    };
private:
    Type type = COUNT;
    Projectile_Manager* projectile_manager;
    float strength = 0;
public:
    Type get_type();
    float get_strength();
    void set_strength(float);
    void set_type(Type);
    void set_projectile_manager(Projectile_Manager*);
    Projectile_Manager* get_projectile_manager();
};

struct Module_Prototype {
    Effects_Prototype effects_prototype;
    float stamina_cost;
    float energy_cost;
    float cooldown;
    float strength;
    Module::Type type;
};

class Shotgun_Module : public Module {
public:
    void activate() override;
    void activate_side_effects() override;
};

class Immortality_Module : public Module {
public:
    void activate() override;
    void activate_side_effects() override;
};

class Invisibility_Module : public Module {
public:
    void activate() override;
    void activate_side_effects() override;
};

class Dash_Module : public Module {
public:
    void activate() override;
    void activate_side_effects() override;
};

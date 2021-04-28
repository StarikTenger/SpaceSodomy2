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
        HP_UP,
        SHOTGUN,
        IMMORTALITY,
        INVISIBILITY,
        DASH,
        COUNT
    };
protected:
    Type type = COUNT;
    Projectile_Manager* projectile_manager;
    float strength = 0;
public:
    static Type get_named_type(std::string name);
    Type get_type();
    float get_strength();
    void set_strength(float);
    void set_type(Type);
    void set_projectile_manager(Projectile_Manager*);
    Projectile_Manager* get_projectile_manager();
    void import_module_prototype(Module_Prototype*);
};

struct Module_Prototype {
    // Effects dindu nuffin as they are not needed for now: 
    // bonus effects are hardcoded
    Effects_Prototype effects_prototype;       
    float stamina_cost = 50;
    //float energy_cost;
    float recharge_time = 5;
    float strength = 20;
    Module::Type type = Module::SHOTGUN;
};

class HpUp_Module : public Module {
public:
    void activate() override;
    void activate_side_effects() override;
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

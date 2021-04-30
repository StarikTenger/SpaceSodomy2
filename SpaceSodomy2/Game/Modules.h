#pragma once
#include "pch.h"
#include "Active_Module.h"
#include "Projectile_Manager.h"
#include <map>

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
    std::map<std::string, float> params;
public:
    static Type get_named_type(std::string name);
    Type get_type();
    float get_param(std::string);
    void set_param(std::string, float);
    void set_type(Type);
    void set_projectile_manager(Projectile_Manager*);
    Projectile_Manager* get_projectile_manager();
    void import_module_prototype(Module_Prototype*);
};

struct Module_Prototype {
    // Effects dindu nuffin as they are not needed for now: supposed to be used for effect-applying shotguns 
    // Bonus effects are hardcoded
    Effects_Prototype effects_prototype;       
    float stamina_cost;
    //float energy_cost;
    float recharge_time;
    std::map<std::string, float> params;
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

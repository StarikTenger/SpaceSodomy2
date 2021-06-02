#pragma once
#include "pch.h"
#include "Active_Module.h"
#include "Projectile_Manager.h"
#include "Event_Manager.h"
#include "Event.h"
#include <map>
#include <unordered_map>
#include "Game_Objects.h"
#include "Damage_Receiver.h"
#include "Wall.h"
#include "Rocket_Manager.h"
#include "Rocket_Brain.h"

struct Module_Prototype;

class Module : public Active_Module {
public:
    enum Type {
        HP_UP,
        SHOTGUN,
        IMMORTALITY,
        INVISIBILITY,
        DASH,
        FORCE,
        BLINK,
        NONE,
        ROCKET,
        COUNT
    };
protected:
    Type type = NONE;
    Projectile_Manager* projectile_manager = nullptr;
    Rocket_Manager* rocket_manager = nullptr;
    std::map<std::string, float> params;
    Game_Objects environment;
    static std::map<int, std::string> names;
    static std::map<std::string, Module::Type> named_types;
public:
    Module() = default;
    Module(Module_Prototype* base);
    virtual ~Module() = default;
    static std::string get_name_by_type(int);
    static Type get_type_by_name(std::string name);
    Type get_type();
    float get_param(std::string);
    void set_param(std::string, float);
    void set_type(Type);
    void set_projectile_manager(Projectile_Manager*);
    Projectile_Manager* get_projectile_manager();
    void set_game_objects(Game_Objects);
    Rocket_Manager* get_rocket_manager();
    void set_rocket_manager(Rocket_Manager*);
};

struct Module_Prototype {
    // Effects dindu nuffin as they are not needed for now: supposed to be used for effect-applying shotguns 
    // Bonus effects are hardcoded
    Effects_Prototype effects_prototype;       
    float stamina_cost = 1000;
    float energy_cost = 1000;
    float recharge_time = 1000;
    std::map<std::string, float> params;
    Module::Type type = Module::NONE;
};

class None_Module : public Module {
public:
    None_Module(Module_Prototype*);
    void activate() override;
};

class HpUp_Module : public Module {
public:
    HpUp_Module(Module_Prototype*);
    float heal;
    void activate() override;
};

class Shotgun_Module : public Module {
public:
    Shotgun_Module(Module_Prototype*);
    int bullet_num;
    float velocity;
    float spread;
    float damage;
    float radius;
    float mass;
    float bullet_hp;
    void activate() override;
};

class Immortality_Module : public Module {
public:
    Immortality_Module(Module_Prototype*);
    float duration;
    void activate() override;
};

class Invisibility_Module : public Module {
public:
    Invisibility_Module(Module_Prototype*);
    float duration;
    void activate() override;
};

class Dash_Module : public Module {
public:
    Dash_Module(Module_Prototype*);
    float vel;
    void activate() override;
};

class Force_Module : public Module {
public:
    Force_Module(Module_Prototype*);
    float radius;
    float vel;
    void activate() override;
};

class Blink_Module : public Module {
public:
    Blink_Module(Module_Prototype*);
    float distance;
    void activate() override;
};

class Rocket_Module : public Module {
public:
    Rocket_Module(Module_Prototype*);
    float force_linear;
    float hp;
    float stamina;
    float stamina_recovery;
    float range;
    float bin_search_accuracy;
    float radius;
    float mass;
    float blast_radius;
    float damage;
    float blast_force;
    void activate() override;
};
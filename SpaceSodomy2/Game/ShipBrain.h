#pragma once
#include "Brain.h"
#include "Projectile.h"
#include <functional>

class ShipBrain : public iBrain {
protected:
    int id;
    // decide which gun and hull the bot wants
    std::string gun_name = "default";
    std::string hull_name = "default";
    std::string left_module_name = "NONE";
    std::string right_module_name = "NONE";
    
public:
    enum class Type {
        EDGAR_BRAIN,
    };
    ShipBrain(CommandModule&, const GameReadable&, int = 0);
    void set_new_id(int);
    std::string get_gun_name();
    std::string get_hull_name();
    std::string get_left_module_name();
    std::string get_right_module_name();

    struct Equip {
        std::string gun_name = "default";
        std::string hull_name = "default";
        std::string left_module_name = "NONE";
        std::string right_module_name = "NONE";

    };
    void suggest_equip(Equip _); // bot can reset them if it wants to
};

class EdgarBrain : public ShipBrain {
private:

    void accelerate_towards_point(Ship* my_ship, b2Vec2 point);
    bool turn_to_angle(Ship* my_ship, float angle);
    void attack(Ship* target, Ship* my_ship );
    void shoot_laser(Ship* target, Ship* my_ship );
    void safety_flight(Ship* my_ship);
    bool is_target_visible(Ship* my_ship, b2Body* target);
    bool is_target_reachable(Ship* my_ship, b2Body* target);
    b2Vec2 calc_volumetric_intersection(b2Vec2 cur_point, float dir_angle, float radius);
    Ship* get_enemy(Ship* my_ship);
    bool get_bonuses(Ship* my_ship);
public:
    EdgarBrain(CommandModule&, const GameReadable&, int = 0);
    void compute_action() final;
};
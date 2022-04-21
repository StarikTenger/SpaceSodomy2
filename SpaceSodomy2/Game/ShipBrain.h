#pragma once
#include "Brain.h"
#include <functional>

class ShipBrain : public iBrain {
protected:
    int id;
public:
    ShipBrain(CommandModule&, const GameReadable&, int = 0);
    void set_new_id(int);
};

class NetworkShipBrain : public ShipBrain {
public:
    NetworkShipBrain(CommandModule&, const GameReadable&, int = 0);

    // Do nothing
    void compute_action() final {};
};

class EdgarBrain : public ShipBrain {
private:
    std::function<b2Vec2(b2Vec2, float)> calc_intersection;

    void move_to_point(Ship* my_ship, b2Vec2 point);
    bool turn_to_angle(Ship* my_ship, float angle);
    void shoot(Ship* target, Ship* my_ship );
    void safety_flight(Ship* my_ship);
    bool is_target_visible(Ship* _ship, Ship* ship);
    b2Vec2 calc_volumetric_intersection(b2Vec2 cur_point, float dir_angle, float radius);
    Ship* get_enemy(Ship* my_ship);
public:
    EdgarBrain(CommandModule&, const GameReadable&, std::function<b2Vec2(b2Vec2, float)> calc_intersection_, int = 0);
    void compute_action() final;
};
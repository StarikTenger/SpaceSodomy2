#pragma once
#include "Rocket.h"
#include "GameReadable.h"
#include "Brain.h"

class Rocket;

class RocketBrain : public iBrain {
private:
    Rocket* rocket;
    float range;
    int bin_search_accuracy;

    // Tells the rocket its direction
    float calculate_dir(b2Body*);

    // Function with zeroes that correspond to hitting the target;
    // Its parameters: target.pos, target.vel, time to reach the target
    float homing_func(b2Vec2 pos, b2Vec2 vel, float time, float accel);
    bool is_targetable(Ship* ship);

    // Tells if the target lies in range of the rocket
    bool is_in_range(b2Vec2 target);

public:
    RocketBrain(CommandModule&, const GameReadable&, Rocket*, float range, int bin_search_accuracy = 20);

    void compute_action() final;
};


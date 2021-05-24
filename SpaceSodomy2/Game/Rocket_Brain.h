#pragma once
#include "Rocket.h"

class Rocket;

class Rocket_Brain {
private:
    Rocket* rocket = nullptr;
    Command_Module* command_module = nullptr;
    Game_Objects environment;
    float range;
    int bin_search_accuracy;

    // Tells the rocket its direction
    float calculate_dir(b2Body*);

    // Function with zeroes that correspond to hitting the target;
    // Its parameters: target.pos, target.vel, time to reach the target
    float homing_func(b2Vec2 pos, b2Vec2 vel, float time, float accel);

public:
    Rocket_Brain(float range, int bin_search_accuracy = 20);
    // Tells if the target lies in range of the rocket
    bool is_in_range(b2Vec2 target);

    void set_rocket(Rocket*);
    void set_game_objects(Game_Objects);
    void set_command_module(Command_Module*);
    void step(float dt);
};


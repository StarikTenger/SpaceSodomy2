#include "pch.h"
#include "Rocket_Brain.h"
#include "Ship.h"

float Rocket_Brain::homing_func(b2Vec2 pos, b2Vec2 vel, float time, float accel) {
    return (b2Dot(pos, pos)) + 
        2.f * (b2Dot(pos , vel)) * time + 
        ((b2Dot(vel, vel)) * time) * time - 
        (1.f / 4.f * accel * accel * time * time * time * time);
}

float Rocket_Brain::calculate_dir(b2Body* body) {
    auto target_pos = body->GetWorldPoint({ 0,0 }) - rocket->get_body()->GetWorldPoint({ 0,0 });
    auto target_vel = body->GetLinearVelocity() - rocket->get_body()->GetLinearVelocity();
    float accel = rocket->get_engine()->get_force_linear() / rocket->get_body()->GetMass();

    float timeLow = 0.f;
    float timeHigh = 1.f + 2.f * sqrt(4.f * (b2Dot(target_vel, target_vel)) / (accel * accel));

    while (homing_func(target_pos, target_vel, timeHigh, accel) > 0) {
        timeHigh *= 2;
    }
    for (int i = 0; i < bin_search_accuracy; i++) {
        if (homing_func(target_pos, target_vel, (timeLow + timeHigh) / 2, accel) >= 0)
            timeLow = (timeLow + timeHigh) / 2;
        else
            timeHigh = (timeLow + timeHigh) / 2;
    }

    return aux::vec_to_angle(target_pos + timeLow * target_vel);
}

bool Rocket_Brain::is_in_range(b2Vec2 target) {
    return (target - rocket->get_body()->GetWorldPoint({ 0,0 })).LengthSquared() < range * range;
}

void Rocket_Brain::set_rocket(Rocket* val) {
    rocket = val;
}
void Rocket_Brain::set_game_objects(Game_Objects val) {
    environment = val;
}
void Rocket_Brain::step(float dt) {
    std::set<Ship*>& ships = *environment.get_ships();
    get_command_module()->set_command(Command_Module::ENGINE_LIN_FORWARD, 1);

    for (auto ship : ships) {
        if (is_targetable(ship)) {
            get_command_module()->set_command(Command_Module::ROCKET_ANGLE, 1);
            get_command_module()->set_rocket_angle(calculate_dir(ship->get_body()));
            break;
        }
    }
}
bool Rocket_Brain::is_targetable(Ship* ship) {
    return
        ship->get_player()->is_hostile_to(rocket->get_combatant())
        && is_in_range(ship->get_body()->GetWorldPoint({ 0,0 }))
        && ship->is_visible();
}


Rocket_Brain::Rocket_Brain(float range_, int bin_search_accuracy_) {
    range = range_;
    bin_search_accuracy = bin_search_accuracy_;
}

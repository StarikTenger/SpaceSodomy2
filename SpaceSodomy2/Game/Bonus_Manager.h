#pragma once
#include "Bonus.h"
#include <deque>
#include <vector>

class Bonus_Manager {
private:
    // First is Types, second is a set
    std::vector<std::vector<b2Vec2>> spawnpoints;
    std::vector<std::vector<bool>> is_spawnpoint_free;
    std::vector<Counter> cooldowns;
    std::deque<Bonus_Def> bonuses_to_create;
    std::vector<Bonus_Prototype> bonus_prototypes;
public:
    Bonus_Manager();
    void add_prototype(Bonus_Prototype);
    void set_cooldown(Bonus::Types, float);
    void step(float dt);
    void add_spawnpoint(Bonus::Types, b2Vec2);
    bool get_next(Bonus_Def&);
    void free_bonus_spawn(Bonus::Types, int id);
    Bonus_Prototype* get_prototype(int type);
};
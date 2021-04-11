#pragma once
#include "Bonus.h"
#include <deque>
#include <stack>
class Bonus_Manager {
private:
    std::map<Bonus::Types, std::vector<b2Vec2>> spawnpoints;
    std::vector<Counter> cooldowns;
    std::stack<std::pair<b2Vec2, Bonus_Def*>> bonuses_to_create;
    std::vector<Bonus_Def> bonus_defs;
public:
    Bonus_Manager();
    void set_cooldown(Bonus::Types);
    void step(float dt);
    void add_spawnpoint(Bonus::Types, b2Vec2);
    std::pair<b2Vec2, Bonus_Def*> get_next();
};
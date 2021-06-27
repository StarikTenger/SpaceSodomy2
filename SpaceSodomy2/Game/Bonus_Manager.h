#pragma once
#include "Bonus.h"
#include <deque>
#include <vector>

class Bonus_Manager {
private:
    // First is Types, second is a set

    struct Spawnlist {
        struct Spawnpoint {
            b2Vec2 pos;
            bool is_free = true;
        };
        Counter cooldown;
        int count = 0;
        int count_max = 0;
        std::vector<Spawnpoint> spawnpoints;
    };
    /*struct Spawnpoint {
        b2Vec2 pos;
        bool is_free = true;
    };
    std::vector<std::vector<Spawnpoint>> spawnpoints;

    struct Global_Spawnability {
        Counter cooldown;
        int count = 0;
        int count_max = 0;
    };
    std::vector<Global_Spawnability> spawnable;*/
    std::vector<Spawnlist> bonus_list;
    std::deque<Bonus_Def> bonuses_to_create;
    std::vector<Bonus_Prototype> bonus_prototypes;
public:
    Bonus_Manager();
    void add_prototype(Bonus_Prototype);
    void set_cooldown(Bonus::Types, float);
    void set_max_count(Bonus::Types, int);
    void step(float dt);
    void add_spawnpoint(Bonus::Types, b2Vec2);
    bool get_next(Bonus_Def&);
    void free_bonus_spawn(Bonus::Types, int id);
    Bonus_Prototype* get_prototype(int type);
};
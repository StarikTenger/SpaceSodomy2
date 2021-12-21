#pragma once
#include "Agent.h"
#include "Game_Map.h"
#include <unordered_map>


// Is unsafe: check for player existence before using
class Respawn_Manager {
public:
    // Algorithms for spawning agents
    // To apply Args ... args, call set_agent_algorithm with args_1 | args_2 | args_3 ...
    // RANDOM spawns agent in random location
    // FIXED_POINT spawns agent in the point given
    enum Spawn_Algorithm {
        RANDOM = 1,
        FIXED_POINT = 2,
    };
private:
    Game_Map* game_map = nullptr;
    struct RespawnData {
        Counter* respawn_timer = nullptr;
        int algo = RANDOM;
        b2Vec2 vec;
        bool is_alive;
    };
    std::unordered_map<Agent*, RespawnData> map;
public:

    void set_game_map(Game_Map*);
    void add_agent(Agent*, Counter*, int spawn_algorithm, b2Vec2 coords = {0,0});
    void rm_agent(Agent*);

    void set_agent_algorithm(Agent*, int spawn_algorithm); // note that if stategy is FIXED_POINT coordinates must be set
    void set_agent_coordinates(Agent*, b2Vec2 coords);
    // WARNING: Client-only!
    void set_agent_is_alive(Agent*, bool);
    
    Counter* get_agent_timer(Agent*);
    bool is_agent_respawn(Agent*);
    bool is_agent_alive(Agent*);
    b2Vec2 respawn_agent(Agent*);
    void kill_agent(Agent*);
};
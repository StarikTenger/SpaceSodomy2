#pragma once
#include "Player.h"
#include "Game_Map.h"
#include <unordered_map>

class Respawn_Manager {
public:
    // Algorithms for spawning agents
    // To apply Args ... args, call set_agent_algorithm with args_1 | args_2 | args_3 ...
    // RANDOM spawns agent in random location
    // FIXED_POINT spawns agent in the point given
    // COMMAND_BASED allows the agent to respawn based on Command_Module::Commands::RESPAWN
    // FORCE_BASED respawns the agent only when force_agent_respawn() is called
    // RANDOM, FIXED_POINT and COMMAND_BASED, FORCE_BASED are exclusive
    enum Spawn_Algorithm {
        RANDOM = 0b0,
        FIXED_POINT = 0b1,
        COMMAND_BASED = 0b00,
        FORCE_BASED = 0b10,
    };
private:
    Game_Map* game_map = nullptr;
    struct RespawnData {
        Counter* respawn_timer = nullptr;
        int algo = RANDOM | COMMAND_BASED;
        b2Vec2 vec;
    };
    std::unordered_map<Agent*, RespawnData> map;
public:
    //void add_agent(Agent*, Counter* respawn_timer, int spawn_algorithm, b2Vec2 coords);
    //void rm_agent(Agent*);

    //void set_agent_cooldown(Agent*, float time);
    //void set_agent_algorithm(Agent*, int spawn_algorithm); // note that if stategy is FIXED_POINT coordinates must be
    //void set_agent_coordinates(Agent*, b2Vec2 coords);

    //void force_agent_respawn(Agent*); // algo & FORCE_BASED must be true
};
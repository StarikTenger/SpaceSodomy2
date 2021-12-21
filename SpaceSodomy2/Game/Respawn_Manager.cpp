#include "pch.h"
#include "Respawn_Manager.h"

void Respawn_Manager::set_game_map(Game_Map* ptr) {
    game_map = ptr;
}
void Respawn_Manager::add_agent(Agent* ptr, Counter* respawn_timer, int algo, b2Vec2 coords) {
    map[ptr].respawn_timer = respawn_timer;
    map[ptr].algo = algo;
    map[ptr].vec = coords;
}
void Respawn_Manager::rm_agent(Agent* ptr) {
    map.erase(ptr);
}
Counter* Respawn_Manager::get_agent_timer(Agent* ptr) {
    return map[ptr].respawn_timer;
}

void Respawn_Manager::set_agent_algorithm(Agent* ptr, int spawn_algorithm) {
    map[ptr].algo = spawn_algorithm;
}
void Respawn_Manager::set_agent_coordinates(Agent* ptr, b2Vec2 coords) {
    map[ptr].vec = coords;
}


bool Respawn_Manager::is_agent_respawn(Agent* ptr) {
    return map[ptr].respawn_timer->get() < 0 && !map[ptr].is_alive;
}
bool Respawn_Manager::is_agent_alive(Agent* ptr) {
    return map[ptr].is_alive;
}

b2Vec2 Respawn_Manager::respawn_agent(Agent* ptr) {
    map[ptr].is_alive = true;
    if (map[ptr].algo & RANDOM) {
        //std::cout << game_map->get_random_coord().x << '\n';
        return game_map->get_random_coord();
    }
    else if (map[ptr].algo & FIXED_POINT){
        return map[ptr].vec;
    }
    std::cerr << "Spawn algorithm error\n";
    return b2Vec2();
}

void Respawn_Manager::kill_agent(Agent* ptr) {
    map[ptr].is_alive = false;
    map[ptr].respawn_timer->set(map[ptr].respawn_timer->get_max());
}

void Respawn_Manager::set_agent_is_alive(Agent* ptr, bool val) {
    map[ptr].is_alive = val;
}

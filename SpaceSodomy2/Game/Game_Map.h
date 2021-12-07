#pragma once
#include "Wall.h"
#include <set>

class Game_Map {
private:
    std::set<Wall*> walls;
    b2Vec2 lower_left_corner = {10000, 10000};
    b2Vec2 upper_right_corner = {-10000, -10000};
public:
    void insert_wall(Wall*);
    bool is_coordinate_allowed(b2Vec2 vec);
    void clear();
    b2Vec2 get_random_coord();
    b2Vec2 get_beam_intersection(b2Vec2 start, float angle);
    std::set<Wall*>* get_walls();
};

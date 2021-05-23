#pragma once
#include <box2d/box2d.h>
#include <set>

class Ship;
class Projectile;
class Bonus;
class Wall;
class Damage_Receiver;

class Game_Objects {
private:
    b2World* physics = nullptr;
    std::set<Ship*>* ships = nullptr;
    std::set<Projectile*>* projectiles = nullptr;
    std::set<Bonus*>* bonuses = nullptr;
    std::set<Wall*>* walls = nullptr;
    std::set<Damage_Receiver*>* damage_receivers = nullptr;

public:
    void set_physics(b2World*);
    void set_ships(std::set<Ship*>*);
    void set_projectiles(std::set<Projectile*>*);
    void set_bonuses(std::set<Bonus*>*);
    void set_walls(std::set<Wall*>*);
    void set_damage_receivers(std::set<Damage_Receiver*>*);
    b2World* get_physics();
    std::set<Ship*>* get_ships();
    std::set<Projectile*>* get_projectiles();
    std::set<Bonus*>* get_bonuses();
    std::set<Wall*>* get_walls();
    std::set<Damage_Receiver*>* get_damage_receivers();
};
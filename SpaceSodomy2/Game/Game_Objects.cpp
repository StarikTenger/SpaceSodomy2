#pragma once
#include "pch.h"
#include "Game_Objects.h"

void Game_Objects::set_physics(b2World* val) {
    physics = val;
}
void Game_Objects::set_ships(std::set<Ship*>* val) {
    ships = val;
}
void Game_Objects::set_projectiles(std::set<Projectile*>* val) {
    projectiles = val;
}
void Game_Objects::set_bonuses(std::set<Bonus*>* val) {
    bonuses = val;
}
void Game_Objects::set_walls(Game_Map* val) {
    walls = val;
}
void Game_Objects::set_damage_receivers(std::set<Damage_Receiver*>* val) {
    damage_receivers = val;
}
b2World* Game_Objects::get_physics() {
    return physics;
}
std::set<Ship*>* Game_Objects::get_ships() {
    return ships;
}
std::set<Projectile*>* Game_Objects::get_projectiles() {
    return projectiles;
}
std::set<Bonus*>* Game_Objects::get_bonuses() {
    return bonuses;
}
Game_Map* Game_Objects::get_walls() {
    return walls;
}
std::set<Damage_Receiver*>* Game_Objects::get_damage_receivers() {
    return damage_receivers;
}
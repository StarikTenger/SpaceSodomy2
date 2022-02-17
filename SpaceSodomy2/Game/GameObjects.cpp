#pragma once
#include "pch.h"
#include "GameObjects.h"

void GameObjects::set_physics(b2World* val) {
    physics = val;
}
void GameObjects::set_ships(std::set<Ship*>* val) {
    ships = val;
}
void GameObjects::set_projectiles(std::set<Projectile*>* val) {
    projectiles = val;
}
void GameObjects::set_bonuses(std::set<Bonus*>* val) {
    bonuses = val;
}
void GameObjects::set_walls(std::set<Wall*>* val) {
    walls = val;
}
void GameObjects::set_damage_receivers(std::set<DamageReceiver*>* val) {
    damage_receivers = val;
}
b2World* GameObjects::get_physics() {
    return physics;
}
std::set<Ship*>* GameObjects::get_ships() {
    return ships;
}
std::set<Projectile*>* GameObjects::get_projectiles() {
    return projectiles;
}
std::set<Bonus*>* GameObjects::get_bonuses() {
    return bonuses;
}
std::set<Wall*>* GameObjects::get_walls() {
    return walls;
}
std::set<DamageReceiver*>* GameObjects::get_damage_receivers() {
    return damage_receivers;
}
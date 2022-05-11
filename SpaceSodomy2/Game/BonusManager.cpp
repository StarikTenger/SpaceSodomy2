#include "pch.h"
#include "BonusManager.h"


BonusManager::BonusManager() :
    spawnpoints(Bonus::Types::COUNT),
    spawnable(Bonus::Types::COUNT),
    bonus_prototypes(Bonus::Types::COUNT) {
    for (int i = 0; i < Bonus::Types::COUNT; i++) {
        spawnable[i].cooldown.set_max(10);
        spawnable[i].cooldown.set(10);
        spawnable[i].cooldown.set_change_vel(1);
        spawnable[i].count = 0;
        spawnable[i].count_max = 100;
    }
}
void BonusManager::set_cooldown(Bonus::Types i, float t) {
    spawnable[i].cooldown.set_max(t);
}
void BonusManager::set_max_count(Bonus::Types i, int t) {
    spawnable[i].count_max = t;
}

void BonusManager::step(float dt) {
    for (int bonus_type = 0; bonus_type < spawnpoints.size(); bonus_type++) {
        if (spawnpoints[bonus_type].size() == 0) {
            continue;
        }
        if (spawnable[bonus_type].count < std::min(spawnable[bonus_type].count_max, (int)spawnpoints[bonus_type].size())) {
            spawnable[bonus_type].cooldown.step(dt);
        }

        if (spawnable[bonus_type].cooldown.get() >= spawnable[bonus_type].cooldown.get_max() - b2_epsilon &&
            spawnable[bonus_type].count < spawnable[bonus_type].count_max) {
            int i = aux::random_int(0, spawnpoints[bonus_type].size() - 1);
            if (!spawnpoints[bonus_type][i].is_free) {
                continue;
            }
            spawnable[bonus_type].cooldown.set(0);
            Bonus_Def def;
            def.pos = spawnpoints[bonus_type][i].pos;
            def.type = static_cast<Bonus::Types>(bonus_type);
            def.set_id(i);
            spawnpoints[bonus_type][i].is_free = false;
            spawnable[bonus_type].count++;
            bonuses_to_create.push_back(def);
        }
    }
}
void BonusManager::add_spawnpoint(Bonus::Types type, b2Vec2 pos) {
    spawnpoints[type].push_back({ pos, true });
}
bool BonusManager::get_next(Bonus_Def& val) {
    if (!bonuses_to_create.size()) {
        return false;
    }
    val = bonuses_to_create.back();

    bonuses_to_create.pop_back();
    return true;

}
void BonusManager::free_bonus_spawn(Bonus::Types type, int id) {
    spawnable[type].count--;
    spawnpoints[type][id].is_free = true;
}

Bonus_Prototype* BonusManager::get_prototype(int type) {
    return &bonus_prototypes[type];
}

void BonusManager::add_prototype(Bonus_Prototype prototypes) {
    bonus_prototypes[prototypes.type] = (prototypes);
}

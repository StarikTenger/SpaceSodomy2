#include "pch.h"
#include "Bonus_Manager.h"


Bonus_Manager::Bonus_Manager() :
    spawnpoints(Bonus::Types::COUNT),
    spawnable(Bonus::Types::COUNT),
    bonus_prototypes(Bonus::Types::COUNT) {
    for (int i = 0; i < Bonus::Types::COUNT; i++) {
        spawnable[i].cooldown.set_max(10);
        spawnable[i].cooldown.set(10);
        spawnable[i].cooldown.set_change_vel(1);
        spawnable[i].count.set(0);
        spawnable[i].count.set_max(10000);
        spawnable[i].count.set_change_vel(0);
    }
}
void Bonus_Manager::set_cooldown(Bonus::Types i, float t) {
    spawnable[i].cooldown.set_max(t);
}
void Bonus_Manager::set_max_count(Bonus::Types i, int t) {
    spawnable[i].count.set_max(t);
}

void Bonus_Manager::step(float dt) {
    for (int bonus_type = 0; bonus_type < spawnpoints.size(); bonus_type++) {
        if (spawnpoints[bonus_type].size() == 0) {
            continue;
        }
        spawnable[bonus_type].cooldown.step(dt);

        if (spawnable[bonus_type].cooldown.get() >= spawnable[bonus_type].cooldown.get_max() - b2_epsilon &&
            spawnable[bonus_type].count.get() >= spawnable[bonus_type].count.get_max() - b2_epsilon) {
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
            spawnable[bonus_type].count.modify(1);
            bonuses_to_create.push_back(def);
        }
    }
}
void Bonus_Manager::add_spawnpoint(Bonus::Types type, b2Vec2 pos) {
    spawnpoints[type].push_back({ pos, true });
}
bool Bonus_Manager::get_next(Bonus_Def& val) {
    if (!bonuses_to_create.size()) {
        return false;
    }
    val = bonuses_to_create.back();

    //std::cout << "exported bonus with type " << val.bonus->type << " & id " << val.get_id() << '\n';
    bonuses_to_create.pop_back();
    return true;

}
void Bonus_Manager::free_bonus_spawn(Bonus::Types type, int id) {
    //std::cout << "bonus_returned with type " << type << "& id " << id <<"\n";
    spawnable[type].count.modify(-1);
    spawnpoints[type][id].is_free = true;
}

Bonus_Prototype* Bonus_Manager::get_prototype(int type) {
    return &bonus_prototypes[type];
}

void Bonus_Manager::add_prototype(Bonus_Prototype prototypes) {
    bonus_prototypes[prototypes.type] = (prototypes);
}

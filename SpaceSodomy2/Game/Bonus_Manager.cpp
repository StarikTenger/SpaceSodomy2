#include "pch.h"
#include "Bonus_Manager.h"


Bonus_Manager::Bonus_Manager() :
    bonus_list(Bonus::Types::COUNT),
    bonus_prototypes(Bonus::Types::COUNT) {
    for (int i = 0; i < Bonus::Types::COUNT; i++) {
        bonus_list[i].cooldown.set_max(10);
        bonus_list[i].cooldown.set(10);
        bonus_list[i].cooldown.set_change_vel(1);
        bonus_list[i].count = 0;
        bonus_list[i].count_max = 100;
    }
}
void Bonus_Manager::set_cooldown(Bonus::Types i, float t) {
    bonus_list[i].cooldown.set_max(t);
}
void Bonus_Manager::set_max_count(Bonus::Types i, int t) {
    bonus_list[i].count_max = t;
}

void Bonus_Manager::step(float dt) {
    for (int bonus_type = 0; bonus_type < bonus_list.size(); bonus_type++) {
        if (bonus_list[bonus_type].spawnpoints.size() == 0) {
            continue;
        }
        if (bonus_list[bonus_type].count < std::min(bonus_list[bonus_type].count_max, (int)bonus_list[bonus_type].spawnpoints.size())) {
            bonus_list[bonus_type].cooldown.step(dt);
        }

        if (bonus_list[bonus_type].cooldown.get() >= bonus_list[bonus_type].cooldown.get_max() - b2_epsilon &&
            bonus_list[bonus_type].count < bonus_list[bonus_type].count_max) {
            int i = aux::random_int(0, bonus_list[bonus_type].spawnpoints.size() - 1);
            if (!bonus_list[bonus_type].spawnpoints[i].is_free) {
                continue;
            }
            bonus_list[bonus_type].cooldown.set(0);
            Bonus_Def def;
            def.pos = bonus_list[bonus_type].spawnpoints[i].pos;
            def.type = static_cast<Bonus::Types>(bonus_type);
            def.set_id(i);
            bonus_list[bonus_type].spawnpoints[i].is_free = false;
            bonus_list[bonus_type].count++;
            bonuses_to_create.push_back(def);
        }
    }
}
void Bonus_Manager::add_spawnpoint(Bonus::Types type, b2Vec2 pos) {
    bonus_list[type].spawnpoints.push_back({ pos, true });
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
    bonus_list[type].count--;
    bonus_list[type].spawnpoints[id].is_free = true;
}

Bonus_Prototype* Bonus_Manager::get_prototype(int type) {
    return &bonus_prototypes[type];
}

void Bonus_Manager::add_prototype(Bonus_Prototype prototypes) {
    bonus_prototypes[prototypes.type] = (prototypes);
}

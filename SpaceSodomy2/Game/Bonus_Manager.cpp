#include "Bonus_Manager.h"


Bonus_Manager::Bonus_Manager() : 
    cooldowns(Bonus::Types::COUNT), 
    spawnpoints(Bonus::Types::COUNT) {
}
void Bonus_Manager::set_cooldown(Bonus::Types i, float t) {
    cooldowns[i].set_max(t);
}
void Bonus_Manager::step(float dt) {
    for (int bonus_type = 0; bonus_type < cooldowns.size(); bonus_type++) {
        if (spawnpoints[bonus_type].size() == 0) {
            continue;
        }
        std::vector<std::pair<b2Vec2, int>> possibles;
        cooldowns[bonus_type].step(dt);
        if (cooldowns[bonus_type].get() <= 0) {
            cooldowns[bonus_type].set(cooldowns[bonus_type].get_max());

            for (int pos_index = 0; pos_index < spawnpoints[bonus_type].size(); pos_index++) {

                if (is_spawnpoint_free[bonus_type][pos_index]) {
                    possibles.push_back(std::make_pair(spawnpoints[bonus_type][pos_index], pos_index));
                }
            }
            if (possibles.size() == 0) {
                continue;
            }
            int i = aux::random_int(0, possibles.size());
            Bonus_Def def;
            def.pos = possibles[i].first;
            def.radius = 0.2;
            def.bonus = &bonus_prototypes[i];
            def.set_id(possibles[i].second);
            is_spawnpoint_free[i][possibles[i].second] = false;
            bonuses_to_create.push_back(def);
        }
    }
}
void Bonus_Manager::add_spawnpoint(Bonus::Types, b2Vec2) {}
bool Bonus_Manager::get_next(Bonus_Def& val) {
    if (!bonuses_to_create.size()) {
        return false;
    }
    val = bonuses_to_create.back();
    bonuses_to_create.pop_back();
    return true;

}
void Bonus_Manager::spawnpoint_freed(Bonus::Types type, int id) {
    is_spawnpoint_free[type][id] = true;
}

void Bonus_Manager::add_prototype(Bonus_Prototype prototypes) {
    bonus_prototypes.push_back(prototypes);
}

#include "pch.h"
#include "Bonus_Manager.h"


Bonus_Manager::Bonus_Manager() :
    cooldowns(Bonus::Types::COUNT),
    spawnpoints(Bonus::Types::COUNT),
    is_spawnpoint_free(Bonus::Types::COUNT),
    bonus_prototypes(Bonus::Types::COUNT) {
    for (int i = 0; i < Bonus::Types::COUNT; i++) {
        cooldowns[i].set_max(10);
        cooldowns[i].set(10);
        cooldowns[i].set_change_vel(1);
    }
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

        if (cooldowns[bonus_type].get() >= cooldowns[bonus_type].get_max() - b2_epsilon) {
            for (int pos_index = 0; pos_index < spawnpoints[bonus_type].size(); pos_index++) { //find all available spawnpoints
                if (is_spawnpoint_free[bonus_type][pos_index]) {
                    possibles.push_back(std::make_pair(spawnpoints[bonus_type][pos_index], pos_index));
                }
            }
            if (possibles.size() == 0) {
                continue;
            }
            // Add bonus to creation deque
            //std::cout << "bonus type " << bonus_type << " their number " << spawnpoints[bonus_type].size() << " possible number "<< possibles.size() << '\n';
            cooldowns[bonus_type].set(0);
            int i = aux::random_int(0, possibles.size() - 1);
            Bonus_Def def;
            def.pos = possibles[i].first;
            def.bonus = &(bonus_prototypes[bonus_type]);
            def.set_id(possibles[i].second);
            is_spawnpoint_free[bonus_type][possibles[i].second] = false;
            bonuses_to_create.push_back(def);
        }
    }
}
void Bonus_Manager::add_spawnpoint(Bonus::Types type, b2Vec2 pos) {
    spawnpoints[type].push_back(pos);
    is_spawnpoint_free[type].push_back(true);
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
    is_spawnpoint_free[type][id] = true;
}

Bonus_Prototype* Bonus_Manager::prototype_by_id(int type) {
    return &bonus_prototypes[type];
}

void Bonus_Manager::add_prototype(Bonus_Prototype prototypes) {
    bonus_prototypes[prototypes.type] = (prototypes);
}

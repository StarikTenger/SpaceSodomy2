#include "pch.h"
#include "Damage_Receiver.h"

Damage_Receiver::Damage_Receiver(b2Body* _body, Counter* _hp) {
    body = _body;
    hp = _hp;
}

b2Body* Damage_Receiver::get_body() {
    return body;
}

Counter* Damage_Receiver::get_hp() {
    return hp;
}

Player* Damage_Receiver::get_last_hit() {
    return last_hit;
}

Player* Damage_Receiver::get_player() {
    return player;
}

Effects* Damage_Receiver::get_effects() {
    return effects;
}


void Damage_Receiver::set_body(b2Body* val) {
    body = val;
}

void Damage_Receiver::set_hp(Counter* val) {
    hp = val;
}

void Damage_Receiver::set_player(Player* val) {
    player = val;
}

void Damage_Receiver::set_effects(Effects* val) {
    effects = val;
}


void Damage_Receiver::damage(float value, Player* hit_by) {
    hp->modify(-value);
    last_hit = hit_by;
}

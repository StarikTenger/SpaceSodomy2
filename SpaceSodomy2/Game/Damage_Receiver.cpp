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

float Damage_Receiver::get_imm_frames() {
    return imm_frames;
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

void Damage_Receiver::set_imm_frames(float val) {
    imm_frames = val;
}


void Damage_Receiver::damage(float value, Player* hit_by) { // Apply IMMORTALITY
    if (!get_player()->is_deals_damage_to(hit_by)) {
        return;
    }
    if (effects && effects->get_effect(Effects::Types::IMMORTALITY)->get_counter()->get() > 0) {
        return;
    }
    if (effects) {
        effects->get_effect(Effects::Types::IMMORTALITY)->get_counter()->set(imm_frames);
    }
    hp->modify(-value);
    if (hit_by != player) {
        last_hit = hit_by;
    }
}

bool Damage_Receiver::apply_effects(Effects_Prototype* val) {
    if (!effects) {
        return false;
    }
    effects->update(val);
    return true;
}

bool Damage_Receiver::add_effect(Effects::Types type, float val) {
    if (!effects) {
        return false;
    }
    effects->get_effect(type)->get_counter()->set(val);
    return true;
}

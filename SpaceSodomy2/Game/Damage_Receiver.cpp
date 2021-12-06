#include "pch.h"
#include "DamageReceiver.h"

DamageReceiver::DamageReceiver(b2Body* _body, Counter* _hp) {
    body = _body;
    hp = _hp;
}

b2Body* DamageReceiver::get_body() {
    return body;
}

Counter* DamageReceiver::get_hp() {
    return hp;
}

Player* DamageReceiver::get_last_hit() {
    return last_hit;
}

Player* DamageReceiver::get_player() {
    return player;
}

Effects* DamageReceiver::get_effects() {
    return effects;
}

float DamageReceiver::get_imm_frames() {
    return imm_frames;
}

void DamageReceiver::set_body(b2Body* val) {
    body = val;
}

void DamageReceiver::set_hp(Counter* val) {
    hp = val;
}

void DamageReceiver::set_player(Player* val) {
    player = val;
}

void DamageReceiver::set_effects(Effects* val) {
    effects = val;
}

void DamageReceiver::set_imm_frames(float val) {
    imm_frames = val;
}


void DamageReceiver::damage(float value, Player* hit_by) { // Apply IMMORTALITY
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

bool DamageReceiver::apply_effects(Effects_Prototype* val) {
    if (!effects) {
        return false;
    }
    effects->update(val);
    return true;
}

bool DamageReceiver::add_effect(Effects::Types type, float val) {
    if (!effects) {
        return false;
    }
    effects->get_effect(type)->get_counter()->set(val);
    return true;
}

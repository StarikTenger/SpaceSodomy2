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

void Damage_Receiver::set_body(b2Body* val) {
    body = val;
}

void Damage_Receiver::set_hp(Counter* val) {
    hp = val;
}

void Damage_Receiver::damage(float value) {
    hp->modify(-value);
}

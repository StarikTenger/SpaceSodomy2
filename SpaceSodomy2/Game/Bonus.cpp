#include "pch.h"
#include "Bonus.h"

void Bonus::set_bonus_prototype(Bonus_Prototype* _bonus_prototype) {
    bonus_prototype = _bonus_prototype;
}
void Bonus::set_body(b2Body* _body) {
    body = _body;
}
Bonus_Prototype* Bonus::get_bonus_prototype() {
    return bonus_prototype;
}
b2Body* Bonus::get_body() {
    return body;
}

Bonus::Types Bonus::get_type() {
    return bonus_prototype->type;
}


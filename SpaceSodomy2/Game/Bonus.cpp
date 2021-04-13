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

Bonus::Types Bonus::get_bonus_type(std::string bonus_name) {
    if (bonus_name == "INSTANT_HP")
        return Bonus::Types::INSTANT_HP;
    if (bonus_name == "INSTANT_STAMINA")
        return Bonus::Types::INSTANT_STAMINA;
    if (bonus_name == "LASER")
        return Bonus::Types::LASER;
    if (bonus_name == "CHARGE")
        return Bonus::Types::CHARGE;
    if (bonus_name == "BERSERK")
        return Bonus::Types::BERSERK;
    if (bonus_name == "IMMORTALITY")
        return Bonus::Types::IMMORTALITY;
    else {
        std::cerr << "incorrect bonus type : " << bonus_name << '\n';
        return Bonus::Types::INSTANT_HP;
    }
}
#include "pch.h"
#include "Bonus.h"

void Bonus::set_type(Bonus::Types type_) {
    type = type_;
}
void Bonus::set_body(b2Body* _body) {
    body = _body;
}
b2Body* Bonus::get_body() {
    return body;
}

Bonus::Types Bonus::get_type() {
    return type;
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
        return Bonus::Types::COUNT;
    }
}
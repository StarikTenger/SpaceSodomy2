#pragma once
#include "pch.h"
#include "Module_Manager.h"


Module_Manager::Module_Manager() : module_prototypes(Module::COUNT) {
}

void Module_Manager::add_prototype(Module_Prototype val) {
    module_prototypes[val.type] = val;
}
Module_Prototype* Module_Manager::get_prototype(Module::Type type) {
    return &module_prototypes[type];
}

Module* Module_Manager::new_module(Module_Prototype* base) {
    switch (base->type) {
    case (Module::HP_UP):
        return new HpUp_Module(base);
    case (Module::SHOTGUN):
        return new Shotgun_Module(base);
    case (Module::IMMORTALITY):
        return new Immortality_Module(base);
    case (Module::INVISIBILITY):
        return new Invisibility_Module(base);
    case (Module::DASH):
        return new Dash_Module(base);
    case (Module::FORCE):
        return new Force_Module(base);
    case (Module::MODULE_BLINK):
        return new Blink_Module(base);
    case(Module::NONE):
        return new None_Module(base);
    case (Module::ROCKET):
        return new Rocket_Module(base);
    }
    std::cout << "new_module: unknown module type\n";
    return new None_Module((base));
}
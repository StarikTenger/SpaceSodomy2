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

Module* Module_Manager::new_module(Module::Type type) {
    switch (type) {
    case (Module::SHOTGUN):
        return new Shotgun_Module;
    case (Module::IMMORTALITY):
        return new Immortality_Module;
    case (Module::INVISIBILITY):
        return new Invisibility_Module;
    case (Module::DASH):
        return new Dash_Module;
    }
}
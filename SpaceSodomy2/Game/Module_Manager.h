#pragma once
#include "Modules.h"
#include <vector>

class Module_Manager {
private:
    std::vector<Module_Prototype> module_prototypes;
public:
    Module_Manager();
    void add_prototype(Module_Prototype);
    Module_Prototype* get_prototype(Module::Type);
    Module* new_module(Module::Type);
};
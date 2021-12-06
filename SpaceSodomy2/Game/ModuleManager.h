#pragma once
#include "Modules.h"
#include <vector>

class ModuleManager {
private:
    std::vector<Module_Prototype> module_prototypes;
public:
    ModuleManager();
    void add_prototype(Module_Prototype);
    Module_Prototype* get_prototype(Module::Type);
    // Effective new operator overload to avoid switch statements later
    Module* new_module(Module_Prototype*);
};
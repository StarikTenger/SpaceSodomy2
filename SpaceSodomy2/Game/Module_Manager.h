#pragma once
#include "Modules.h"
#include <vector>

class Module_Manager {
private:
    std::vector<Module_Prototype> module_prototypes;
public:
    Module_Manager();
    void add_prototype(Module_Prototype);
    Module_Manager* get_prototype(Module::Type);
};
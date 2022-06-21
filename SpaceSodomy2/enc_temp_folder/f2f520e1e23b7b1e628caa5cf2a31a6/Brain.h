#pragma once
#include "CommandModule.h"
#include "GameReadable.h"

class iBrain {
public:
    iBrain(CommandModule&, const GameReadable&);
    CommandModule* get_command_module();
    virtual void compute_action() = 0;
    virtual ~iBrain() = default;
protected:
    CommandModule& command_module;
    const GameReadable& environment;
};

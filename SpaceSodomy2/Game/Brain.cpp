#include "pch.h"
#include "Brain.h"


iBrain::iBrain(CommandModule& _, const GameReadable& __) : command_module(_), environment(__) {};
CommandModule* iBrain::get_command_module() { return &command_module; }


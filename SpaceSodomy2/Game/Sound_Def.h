#pragma once
#include <box2d/box2d.h>
#include <string>

struct Sound_Def {
	std::string name = "_";
	b2Body* body = nullptr;
};
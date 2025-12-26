#pragma once
#include "core_interface.h"

//ECS component of the engine
class Component {
public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void shutdown() = 0;
};
#pragma once

//Core interface most classes in the engine share
class CoreInterface {

public:
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void shutdown() = 0;

};
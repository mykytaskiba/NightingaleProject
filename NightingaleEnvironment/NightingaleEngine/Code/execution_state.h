#pragma once
#include "defines.h"


class GameObject;
class RenderNode;

struct ExecutionState {

    string lastCommand{ "No command" };

    map<string, string> defines{};

    float extract_float_with_default(string const& key, float default_val) const;

    //log
    bool bCreateLog{ false };
    string log{};
    string errorLog{};

    GameObject* pGameObject;
    RenderNode* pRenderNode;

};

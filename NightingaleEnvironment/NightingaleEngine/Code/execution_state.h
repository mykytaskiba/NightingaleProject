#pragma once
#include "defines.h"


class GameObject;
class RenderNode;

struct ExecutionState {

    string lastCommand{ "No command" };

    map<string, string> defines{};

    //log
    bool bCreateLog{ false };
    string log{};
    string errorLog{};

    GameObject* pGameObject;
    RenderNode* pRenderNode;


    //loader values
    float loaderScale{ 1.0f };
};

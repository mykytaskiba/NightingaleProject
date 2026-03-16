#include "pch.h"
#include "scene_browser.h"
#include "engine_functions.h"

void SceneHierarchy::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("Scene Hierarchy", &bRemainOpen);


    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}


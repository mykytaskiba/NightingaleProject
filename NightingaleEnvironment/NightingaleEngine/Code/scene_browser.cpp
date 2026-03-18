#include "pch.h"
#include "scene_browser.h"
#include "engine_functions.h"
#include "scene.h"
#include "game_object.h"

void SceneHierarchy::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("Scene Hierarchy", &bRemainOpen);


    if (ImGui::BeginMenu("Create")) {
        if (ImGui::Button("GameObject")) {
            EngineFunctions::ExecuteCommand("create_gameobject gameobject");
        }


        ImGui::EndMenu();
    }

    ImGui::Separator();

    drawSceneTree(EngineFunctions::scene().get_root());

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

void SceneHierarchy::drawSceneTree(GameObject* pGameObject)
{
    if (pGameObject == nullptr) {
        assert(false);
        return;
    }
    if (ImGui::TreeNode(pGameObject->getAlias().c_str())) {
        for (GameObject* pChild : pGameObject->get_children()) {
            drawSceneTree(pChild);
        }
        ImGui::TreePop();
    }
}


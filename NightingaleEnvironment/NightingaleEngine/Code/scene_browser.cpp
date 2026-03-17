#include "pch.h"
#include "scene_browser.h"
#include "engine_functions.h"
#include "scene.h"

void SceneHierarchy::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("Scene Hierarchy", &bRemainOpen);


    if (ImGui::BeginMenu("+")) {
        if (ImGui::Button("GameObject")) {
            EngineFunctions::ExecuteCommand("create_gameobject gameobject");
        }


        ImGui::EndMenu();
    }


    ImGui::TreePush("scene_tree");
    

    EngineFunctions::scene().execute_on_root(
        [](GameObject& gameObject) {
            
            if (ImGui::TreeNode(gameObject.getAlias().c_str())) {
                ImGui::TreePop();
            }
        }
    );
    ImGui::TreePop();

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}


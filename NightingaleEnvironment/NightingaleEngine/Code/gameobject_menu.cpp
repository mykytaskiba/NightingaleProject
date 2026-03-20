#include "pch.h"
#include "gameobject_menu.h"
#include "game_object.h"
#include "transform_menu.h"
#include "render_node_menu.h"
#include "physics_body_menu.h"
#include "camera_controller.h"
#include "scene.h"
#include "engine_functions.h"

void GameObjectMenu::render_update(GameObject* pGameObject)
{
    if (pGameObject == nullptr) {
        assert(false);
        return;
    }

    CameraController* pCameraController = dynamic_cast<CameraController*>(pGameObject);
    if (pCameraController != nullptr) {
        ImGui::Text("Camera Controller");
        ImGui::Text("No Modifiable Properties");
        return;
    }

    if (EngineFunctions::scene().get_root() == pGameObject) {
        ImGui::Text("Scene Root");
        ImGui::Text("No Modifiable Properties");
        return;
    }
    


    string aliasStr{ pGameObject->getAlias() };
    if (ImGui::InputText("Alias", &aliasStr)) {
        pGameObject->setAlias(aliasStr);
    }

    ImGui::Separator();
    
    ImGuiTreeNodeFlags subViewsFlags = ImGuiTreeNodeFlags_NoTreePushOnOpen;

    if (ImGui::TreeNodeEx("Transform ", subViewsFlags)) {
        TransformMenu::render_update(pGameObject->getTransform());
    }

    ImGui::Separator();

    if (ImGui::TreeNodeEx("Render Properties", subViewsFlags)) {
        RenderNodeMenu::render_update(*pGameObject);
    }

    ImGui::Separator();

    if (ImGui::TreeNodeEx("Physics Properties", subViewsFlags)) {
        PhysicsBodyMenu::render_update(*pGameObject);
    }
}

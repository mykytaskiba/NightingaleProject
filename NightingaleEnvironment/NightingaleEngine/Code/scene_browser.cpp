#include "pch.h"
#include "scene_browser.h"
#include "engine_functions.h"
#include "scene.h"
#include "game_object.h"
#include "gameobject_menu.h"
#include "loader.h"
#include "properties.h"
#include "property_visitor.h"

void SceneHierarchy::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("Scene Hierarchy", &bRemainOpen);

    ImGui::Checkbox("Show GUIDs", &m_bShowGUID);

    if (ImGui::BeginMenu("Create")) {
        EngineFunctions::factoryGameObject().foreach_key([](std::string const& key) {
                if (ImGui::Button(key.c_str())) {
                    std::string executedCommand = "create_gameobject " + key;
                    EngineFunctions::ExecuteCommand(executedCommand);
                }
            }
        );
        ImGui::EndMenu();
    }

    if (ImGui::Button("Delete")) {
        EngineFunctions::ExecuteCommand("delete_gameobject");
    }


    if (ImGui::Button("Clear Scene")) {
        EngineFunctions::ExecuteCommand("clear_scene");
    }

    ImGui::Separator();

    drawSceneTree(EngineFunctions::scene().getRoot());

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

    std::string gameObjectAliasString = pGameObject->getAlias();
    if (m_bShowGUID) {
        gameObjectAliasString += ":";
    }
    else {
        gameObjectAliasString += "##";
    }
    gameObjectAliasString += pGameObject->getGUID().string();

    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow  | ImGuiTreeNodeFlags_OpenOnDoubleClick;

    GUID selectedGUID = EngineFunctions::execution_state().getSelectedGUID();
    if (pGameObject->getGUID() == selectedGUID) {
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    }

    bool bNodeOpen = ImGui::TreeNodeEx(gameObjectAliasString.c_str(), nodeFlags);
    bool bNodeClicked = ImGui::IsItemClicked(0);
    bool bNodeRightClicked = ImGui::IsItemClicked(1);


    if (bNodeOpen) {
        for (GameObject* pChild : pGameObject->getChildren()) {
            drawSceneTree(pChild);
        }
        ImGui::TreePop();
    }
    
    if (bNodeClicked) {
        EngineFunctions::execution_state().setSelectedGUID(pGameObject->getGUID());
    }



}

void Inspector::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("Inspector", &bRemainOpen);

    GUID selectedGUID = EngineFunctions::execution_state().getSelectedGUID();

    std::string selectedGUIDStr = "Selected GUID: " + selectedGUID.string();
    ImGui::Text(selectedGUIDStr.c_str());

    ImGui::Separator();

    GameObject* pGameObject = EngineFunctions::scene().findObject(selectedGUID);
    if (pGameObject == nullptr) {
        ImGui::Text("Object not found");
    }
    else {
        GameObjectMenu::render_update(pGameObject);

        if (ImGui::Button("DebugJSON")) {

            nlohmann::json json = GameObject::JSONRepresentation::serialize(*pGameObject);
            
            Loader::saveToFile("jsonTest/gameobject.json", json);
        }
    }

    if (pGameObject != nullptr) {
        ImGui::Separator();
        ImGui::Text("Properties");

        PropertyMenuVisitor menuVisitor{};
        pGameObject->properties(menuVisitor);
    }
    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

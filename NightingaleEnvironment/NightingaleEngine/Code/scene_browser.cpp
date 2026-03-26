#include "pch.h"
#include "scene_browser.h"
#include "engine_functions.h"
#include "scene.h"
#include "game_object.h"
#include "gameobject_menu.h"
#include "loader.h"
#include "property_visitor.h"
#include "property_menu.h"

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

    ImGui::Separator();

    if (ImGui::Button("Serialize Scene")) {

        nlohmann::json json;
        Scene& scene = EngineFunctions::scene();
        if (scene.json()->serialize(json)) {
            Loader::saveFile("jsonTest/scene.json", json);
        }

    }

    if (ImGui::Button("Deserialize Scene")) {

        nlohmann::json json;
        Scene& scene = EngineFunctions::scene();
        bool bReadSuccess = Loader::readFile("jsonTest/scene.json", json);

        if (bReadSuccess) {
            scene.json()->deserialize(json);
        }
    }

    ImGui::Separator();
    ImGui::Text("Scene Properties");

    PropertyMenuVisitor menuVisitor{};
    Scene& scene = EngineFunctions::scene();
    scene.properties(menuVisitor);

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

        if (ImGui::Button("Serialize JSON")) {
            
            nlohmann::json json;
            if (pGameObject->json()->serialize(json)) {
                Loader::saveFile("jsonTest/gameobject.json", json);
            }
            
        }

        if (ImGui::Button("Deserialize JSON")) {

            nlohmann::json json;
            bool bReadSuccess = Loader::readFile("jsonTest/gameobject.json", json);

            GameObject* pDeserialized{ nullptr };
            GameObject temp{};


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

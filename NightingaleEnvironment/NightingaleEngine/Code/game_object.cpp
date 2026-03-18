#include "pch.h"
#include "game_object.h"
#include "engine_functions.h"
#include "transform_menu.h"


void GameObject::execute_on_hierarchy(TGameObjectFunc functor)
{
    (functor)(*this);

    execute_on_children(functor);
}

void GameObject::execute_on_children(TGameObjectFunc functor)
{
    for (auto it = m_children.begin(); it != m_children.end(); ++it) {
        (functor)(**it);
    }
}

void GameObject::render_update_debugMenu()
{
    string selectedGUIDSTR = "GUID: " + m_guid.string();
    ImGui::Text(selectedGUIDSTR.c_str());

    string aliasStr{ m_alias };
    ImGui::InputText("Alias", &aliasStr);
    
    setAlias(aliasStr);

    ImGui::Separator();
    
    TransformMenu::render_update(m_transform);

    ImGui::Separator();

    if (m_pRenderNode == nullptr) {
        ImGui::Text("No Render Node");

        if (ImGui::Button("Attach RenderMesh Node")) {
            EngineFunctions::execution_state().pGameObject = this;
            EngineFunctions::ExecuteCommand("attach_rendermesh cube_mesh");
        }
    }


}

GameObject::GameObject()
{
    m_guid = GUID::Generate();
}

GameObject::GameObject(std::string const& alias) : m_alias(alias)
{
    m_guid = GUID::Generate();
}

void GameObject::sync_to_physics()
{
    if (m_pPhysicsBody == nullptr) {
        return;
    }
    m_transform.position = m_pPhysicsBody->position;
}

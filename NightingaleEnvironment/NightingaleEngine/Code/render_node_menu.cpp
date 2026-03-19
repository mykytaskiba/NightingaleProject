#include "pch.h"
#include "render_node_menu.h"
#include "game_object.h"
#include "engine_functions.h"
#include "render_mesh.h"
#include "imgui_helpers.h"
#include "asset_manager.h"

void RenderNodeMenu::render_update(GameObject& gameObject)
{
	if (!gameObject.hasRenderNode()) {
        ImGui::Text("No Render Node");

        if (ImGui::Button("Attach RenderMesh Node")) {
            EngineFunctions::execution_state().pGameObject = &gameObject;
            EngineFunctions::ExecuteCommand("attach_rendermesh cube_mesh");
        }
        return;
	}

    RenderMeshNode* pRenderMesh = dynamic_cast<RenderMeshNode*>(gameObject.getRenderNode());
    if (pRenderMesh != nullptr) {
        render_update(pRenderMesh);
        return;
    }
}

void RenderNodeMenu::render_update(RenderMeshNode* pRenderMesh)
{
    if (pRenderMesh == nullptr) {
        return;
    }

    static RenderMeshNode* pLastOpened{ nullptr };
    static std::string meshCache{};

    if (pLastOpened != pRenderMesh) {
        pLastOpened = pRenderMesh;
        meshCache = "";
    }

    ImGui::Text("Render Mesh");

    Color color = pRenderMesh->getColor();
    if (ImGuiHelpers::ColorPicker4("Color", color)) {
        pRenderMesh->setColor(color);
    }

    if (ImGui::InputText("Mesh", &meshCache)) {
        if (AssetManager<Mesh>::has(meshCache)) {
            Mesh* pMesh = AssetManager<Mesh>::retrieve(meshCache);
            pRenderMesh->setMesh(pMesh);
        }
    }
    

}

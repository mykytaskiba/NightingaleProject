#include "pch.h"
#include "resource_browsers.h"
#include "asset_manager.h"
#include "render_shader.h"
#include "mesh.h"

void ShaderResourceBrowser::render_update()
{

    bool bRemainOpen{ true };

    ImGui::Begin("Shader Browser", &bRemainOpen);

    AssetManager<RenderShader>::execute_key(
        [this](std::string const& key, RenderShader* pShader) {
            ImGui::Text(key.c_str());
        }
    );

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

void MeshBrowser::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("Mesh Browser", &bRemainOpen);

    AssetManager<Mesh>::execute_key(
        [this](std::string const& key, Mesh* pMesh) {
            if (!ImGui::CollapsingHeader(key.c_str())) return;

            ImGui::Text("MESH");
            ImGui::Text("MESH");
            ImGui::Text("MESH");
        }
    );

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

#include "pch.h"
#include "resource_browsers.h"
#include "asset_manager.h"
#include "render_shader.h"

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

#include "pch.h"
#include "resource_browsers.h"

void ShaderResourceBrowser::render_update()
{

    bool bRemainOpen{ true };

    ImGui::Begin("Shader Browser", &bRemainOpen);

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

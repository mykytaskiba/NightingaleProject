#include "pch.h"
#include "fileloading_panel.h"
#include "loader.h"

void FileSavingPanel::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("File Saving Debug", &bRemainOpen);

    ImGui::InputText("Path", &m_path);
    ImGui::InputTextMultiline("Contents", &m_contents);
    
    ImGui::Checkbox("Create Directories", &m_bCreateDirectories);
    ImGui::SameLine();
    ImGui::Checkbox("Overwrite File", &m_bOverwriteFile);


    if (ImGui::Button("Save File")) {
        FileCreationFlags flags = FileCreationFlags::None;
        if (m_bCreateDirectories) {
            flags |= FileCreationFlags::CreateDirectory;
        }
        if (m_bOverwriteFile) {
            flags |= FileCreationFlags::Overwrite;
        }
        m_bLastResult = Loader::saveToFile(std::filesystem::path{ m_path }, m_contents, flags);
    }

    ImGui::SameLine();
    if (m_bLastResult) {
        ImGui::Text("File Saved");
    }
    else {
        ImGui::Text("File Not Saved");
    }

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

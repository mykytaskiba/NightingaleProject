#include "pch.h"
#include "fileloading_panel.h"
#include "loader.h"
#include "ngjson.h"

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

void JSONUpgraderPanel::render_update()
{
    bool bRemainOpen{ true };

    ImGui::Begin("File Saving Debug", &bRemainOpen);

    ImGui::Text("Debug folder {}", m_path);

    if (ImGui::Button("Create Sample JSONs")) {
        
        {//v1
            nlohmann::json jsonTemp;
            jsonTemp[JSON_META_KEY][JSON_VERSION_KEY] = 1;
            jsonTemp[JSON_META_KEY][JSON_TYPE_KEY] = SampleObject::c_JSONType;
            
            jsonTemp["original_float"] = 0.0f;
            jsonTemp["deleted_float"] = 1.0f;
            jsonTemp["original_string_var"] = "ABCD";
            jsonTemp["deleted_int"] = 10;
            Loader::saveToFile(m_path + "sample_v1", jsonTemp);
        }
        {//v2
            nlohmann::json jsonTemp;
            jsonTemp[JSON_META_KEY][JSON_VERSION_KEY] = 2;
            jsonTemp[JSON_META_KEY][JSON_TYPE_KEY] = SampleObject::c_JSONType;

            jsonTemp["original_float"] = 0.0f;
            jsonTemp["deleted_int"] = 10;
            jsonTemp["original_string_var"] = "ABCD";
            Loader::saveToFile(m_path + "sample_v2", jsonTemp);
        }
        {//v3
            nlohmann::json jsonTemp;
            jsonTemp[JSON_META_KEY][JSON_VERSION_KEY] = 3;
            jsonTemp[JSON_META_KEY][JSON_TYPE_KEY] = SampleObject::c_JSONType;

            jsonTemp["original_float"] = 0.0f;
            jsonTemp["deleted_int"] = 10;
            jsonTemp["renamed_string_var"] = "ABCD";
            Loader::saveToFile(m_path + "sample_v3", jsonTemp);
        }
        {//v4
            nlohmann::json jsonTemp;
            jsonTemp[JSON_META_KEY][JSON_VERSION_KEY] = 4;
            jsonTemp[JSON_META_KEY][JSON_TYPE_KEY] = SampleObject::c_JSONType;

            jsonTemp["original_float"] = 0.0f;
            jsonTemp["deleted_int"] = 10;
            jsonTemp["renamed_string_var"] = "ABCD";
            jsonTemp["added_int"] = 15;
            Loader::saveToFile(m_path + "sample_v4", jsonTemp);
        }
        {//wrong type
            nlohmann::json jsonTemp;
            jsonTemp[JSON_META_KEY][JSON_VERSION_KEY] = 1;
            jsonTemp[JSON_META_KEY][JSON_TYPE_KEY] = "json.wrongobject";

            jsonTemp["wrong_data"] = 0.0f;
            jsonTemp["wrong_data_2"] = 10;
            jsonTemp["wrong_data_3"] = "ABCD";
            jsonTemp["wrong_data_4"] = 15;
            Loader::saveToFile(m_path + "sample_wrong_type", jsonTemp);
        }
        {//high version
            nlohmann::json jsonTemp;
            jsonTemp[JSON_META_KEY][JSON_VERSION_KEY] = 10;
            jsonTemp[JSON_META_KEY][JSON_TYPE_KEY] = SampleObject::c_JSONType;

            jsonTemp["future_data_1"] = 15;
            jsonTemp["future_data_2"] = "TEST";
            jsonTemp["future_data_3"] = 1.5f;
            Loader::saveToFile(m_path + "sample_v10_highversion", jsonTemp);
        }
    }

    if (ImGui::Button("Convert JSONs")) {
        performConversion(m_path + "sample_v1");
        performConversion(m_path + "sample_v2");
        performConversion(m_path + "sample_v3");
        performConversion(m_path + "sample_v4");
        performConversion(m_path + "sample_wrong_type");
        performConversion(m_path + "sample_v10_highversion");
    }

    ImGui::End();

    if (!bRemainOpen && m_bActive) {
        toggle();
    }
}

void JSONUpgraderPanel::performConversion(std::string path)
{
    SampleObject sample;

    std::ifstream f(path);

    if (!f.is_open()) {
        return;
    }
    nlohmann::json data;
    try {
        // Parse the JSON data from the file stream
        data = nlohmann::json::parse(f);
    }
    catch (nlohmann::json::parse_error& e) {
        return;
    }

    SampleObject::JSONRepresentation sampleObjectJson(sample, data);

    sampleObjectJson.executeOperation(JSONMode::Deserialize);

    std::filesystem::path export_path = std::filesystem::path{ path };
    std::string filename = export_path.filename().string();
    export_path.replace_filename(filename + "_result");

    Loader::saveToFile(export_path, sampleObjectJson.getJSON());
}

bool JSONUpgraderPanel::SampleObject::upgradeJSON(JSONUpgrader& upgrader)
{
    upgrader.deleted_variable(1, "deleted_float");
    upgrader.renamed_variable(2, "original_string_var", "renamed_string_var");
    upgrader.added_variable(3, "added_int", 15);
    upgrader.deleted_variable(4, "deleted_int");

    return true;
}

bool JSONUpgraderPanel::SampleObject::jsonOperation(JSONOperation& operation)
{
    operation.link("added_int", m_addedInt);
    operation.link("original_float", m_originalFloat);
    operation.link("renamed_string_var", m_renamedVariable);
    return true;
}

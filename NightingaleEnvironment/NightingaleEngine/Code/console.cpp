#include "pch.h"
#include "console.h"
#include "input.h"
#include "engine_functions.h"
#include "core_commands.h"
#include "loader_commands.h"
#include "gameobject_commands.h"

void Console::init()
{
}

void Console::tick()
{
    bool bShowConsolePressed = Input::KeyClicked(Key::Tilda);
    if (bShowConsolePressed) {
        m_consoleShown = !m_consoleShown;
        m_consoleBuffer.clear();
        ImGui::SetNextWindowFocus();
    }

    if (m_consoleShown) {
        int screenWidth = EngineFunctions::ScreenWidth();
        
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(screenWidth, m_consoleHeight));

        bool hasPreviousExecutedCommands = (m_executedCommands.size() != 0);
        

        if (Input::KeyClicked(Key::UpArrow) && hasPreviousExecutedCommands) {
            uint commandIndex = m_executedCommands.size() - m_executedCommandBrowser - 1;
                m_consoleBuffer = m_executedCommands[commandIndex];

                if (m_executedCommandBrowser + 1u < m_executedCommands.size()) {
                    m_executedCommandBrowser++;
                }
        }
        if (Input::KeyClicked(Key::DownArrow) && hasPreviousExecutedCommands) {
            if (m_executedCommandBrowser > 0) {
                m_executedCommandBrowser--;
            }
            
            uint commandIndex = m_executedCommands.size() - m_executedCommandBrowser - 1;
            m_consoleBuffer = m_executedCommands[commandIndex];
        }

        ImGui::Begin("Console", NULL, 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoTitleBar);

        if (bShowConsolePressed) {
            ImGui::SetKeyboardFocusHere();
        }
        if (ImGui::InputText("<--", &m_consoleBuffer)) {
            m_executedCommandBrowser = 0;
        }
        ImGui::Text(m_consoleMessage.c_str());
        ImGui::End();

        //both keys are down and one of them was just clicked
        bool commandEntered =
            (Input::KeyDown(Key::Shift) && Input::KeyClicked(Key::Enter))
            ||
            (Input::KeyClicked(Key::Shift) && Input::KeyDown(Key::Enter));
            

        if (commandEntered) {
            string fullCommand = string(m_consoleBuffer);
            assert(ScriptingEnvironment::getInstance() != nullptr);
            ScriptingEnvironment& scriptingEnv = *ScriptingEnvironment::getInstance();
            ExecutionResult executionResult = scriptingEnv.execute(fullCommand);
            if (executionResult.bSuccess) {
                m_consoleBuffer.clear();
                m_executedCommands.push_back(fullCommand);
                m_executedCommandBrowser = 0;
            }
            m_consoleMessage = executionResult.message;
        }
    }
}



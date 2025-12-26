#include "pch.h"
#include "debug_ui.h"

#include "window.h"
#include "input.h"

DebugUI::DebugUI()
{
}

void DebugUI::init(Window& window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.GetNativeWindow(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}

void DebugUI::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DebugUI::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGui::ShowDemoWindow(); // Show demo window! :)
    /*
    ImGui::Begin("Test Window", NULL);
    ImGui::Button("Test Button", ImVec2(200, 25));
    ImGui::End();

    ImGui::Begin("Input Debug", NULL);

    ImGui::Text("Mouse Pos %.1f,%.1f", Input::MousePos()[0], Input::MousePos()[1]);
    ImGui::Text("Mouse Delta %.1f,%.1f", Input::MouseDelta()[0], Input::MouseDelta()[1]);
    ImGui::Text("Mouse Wheel %.1f", Input::MouseScroll());

    ImGui::Text("Left Mouse Down: %s", 
        Input::MouseDown(MouseButton::Left) ? "true" : "false");

    ImGui::Text("Right Mouse Down: %s",
        Input::MouseDown(MouseButton::Right) ? "true" : "false");

    ImGui::Text("Middle Mouse Down: %s",
        Input::MouseDown(MouseButton::Middle) ? "true" : "false");

    ImGui::Text("Key Enter Down: %s",
        Input::KeyDown(Key::Enter) ? "true" : "false");

    ImGui::End();*/

    
}

void DebugUI::endFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#include "pch.h"
#include "physics_body_menu.h"
#include "game_object.h"
#include "ngphys.h"
#include "engine_functions.h"
#include "imgui_helpers.h"

void PhysicsBodyMenu::render_update(GameObject& gameObject)
{
    if (!gameObject.hasPhysicsBody()) {
        ImGui::Text("No Physics Body");

        if (ImGui::Button("Attach Physics Body")) {
            EngineFunctions::execution_state().setSelectedGUID(gameObject.getGUID());
            EngineFunctions::ExecuteCommand("attach_physicsbody");
        }
        return;
    }

    if (gameObject.getPhysicsBody() != nullptr) {
        render_update(*gameObject.getPhysicsBody());
    }


}

void PhysicsBodyMenu::render_update(PhysicsBody& physBody)
{

    bool bUseGravity = physBody.useGravity();
    if (ImGui::Checkbox("Use Gravity", &bUseGravity)) {
        physBody.setGravity(bUseGravity);
    }

    /*
    Vector3 position = physBody.getPosition();
    if (ImGuiHelpers::Vector3Input("Position", position)) {
        physBody.setPosition(position);
    }*/

    Vector3 velocity = physBody.getVelocity();
    if (ImGuiHelpers::Vector3Input("Velocity", velocity)) {
        physBody.setVelocity(velocity);
    }

    AxisAlignedBox AABB = physBody.getLocalBox();
    if (ImGuiHelpers::AxisAlignedBoxInput("Bounding box", AABB)) {
        physBody.setLocalBox(AABB);
    }
}

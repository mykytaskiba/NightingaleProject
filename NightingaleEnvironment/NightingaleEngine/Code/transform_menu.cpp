#include "pch.h"
#include "transform_menu.h"
#include "ngmath.h"
#include "imgui_helpers.h"

void TransformMenu::render_update(Transform& transform)
{
	ImGuiHelpers::Vector3Input("Position", transform.position, 0.25f);

}

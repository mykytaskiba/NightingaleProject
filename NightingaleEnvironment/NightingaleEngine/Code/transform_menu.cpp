#include "pch.h"
#include "transform_menu.h"
#include "ngmath.h"
#include "imgui_helpers.h"
#include "transform.h"

void TransformMenu::render_update(Transform& transform)
{
	ImGuiHelpers::Vector3Input("Position", transform.position, 0.25f);

	ImGuiHelpers::Vector3Input("Scale", transform.scale, 0.1f);

}

#pragma once
#include "json.hpp"
#include "vector.h"
#include "quaternion.h"
#include "axis_aligned_box.h"



void to_json(nlohmann::json& json, const AxisAlignedBox& box);
void from_json(nlohmann::json const& json, AxisAlignedBox& box);

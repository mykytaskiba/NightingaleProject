#pragma once
#include "json.hpp"
#include "vector.h"
#include "quaternion.h"
#include "axis_aligned_box.h"

void to_json(nlohmann::json& json, const Vector3& vec);
void from_json(nlohmann::json const& json, Vector3& vec);

void to_json(nlohmann::json& json, const Vector4& vec);
void from_json(nlohmann::json const& json, Vector4& vec);

void to_json(nlohmann::json& json, const Quaternion& quat);
void from_json(nlohmann::json const& json, Quaternion& quat);

void to_json(nlohmann::json& json, const AxisAlignedBox& box);
void from_json(nlohmann::json const& json, AxisAlignedBox& box);
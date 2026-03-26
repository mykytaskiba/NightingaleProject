#pragma once

//NGMath is safe to include here
#include "json.hpp"
#include "vector.h"
#include "quaternion.h"

void to_json(nlohmann::json& json, const Vector3& vec);
void from_json(nlohmann::json const& json, Vector3& vec);

void to_json(nlohmann::json& json, const Vector4& vec);
void from_json(nlohmann::json const& json, Vector4& vec);

void to_json(nlohmann::json& json, const Quaternion& quat);
void from_json(nlohmann::json const& json, Quaternion& quat);
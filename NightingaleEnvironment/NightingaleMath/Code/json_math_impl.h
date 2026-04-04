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

#include "property_visitor.h"

template<>
inline void IPropertyVisitor::operator() < Vector3 > (std::string const& key, Vector3& value) {
	handle_vector3(key, value[0], value[1], value[2]);
}
template<>
inline void IPropertyVisitor::operator() < Vector4 > (std::string const& key, Vector4& value) {
	handle_vector4(key, value[0], value[1], value[2], value[3]);
}
template<>
inline void IPropertyVisitor::operator() < Quaternion > (std::string const& key, Quaternion& value) {
	handle_vector4(key, value.w, value.x, value.y, value.z);
}
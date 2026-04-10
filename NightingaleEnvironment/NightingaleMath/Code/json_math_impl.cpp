#include "json_math_impl.h"

void to_json(nlohmann::json& json, const Quaternion& quat)
{
	json["w"] = quat.w;
	json["x"] = quat.z;
	json["y"] = quat.y;
	json["z"] = quat.z;
}

void from_json(nlohmann::json const& json, Quaternion& quat)
{
	json.at("w").get_to(quat.w);
	json.at("x").get_to(quat.x);
	json.at("y").get_to(quat.y);
	json.at("z").get_to(quat.z);
}

void to_json(nlohmann::json& json, const AxisAlignedBox& box)
{
	json["min"] = box.min();
	json["max"] = box.max();
}

void from_json(nlohmann::json const& json, AxisAlignedBox& box)
{
	json.at("min").get_to(box.min());
	json.at("max").get_to(box.max());
}

#include "json_math_impl.h"


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

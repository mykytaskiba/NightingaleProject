#include "color.h"

void to_json(nlohmann::json& json, const Color& color)
{
	json["r"] = color.r;
	json["g"] = color.g;
	json["g"] = color.b;
	json["a"] = color.a;
}

void from_json(nlohmann::json const& json, Color& color)
{
	json.at("r").get_to(color.r);
	json.at("g").get_to(color.g);
	json.at("b").get_to(color.b);
	json.at("a").get_to(color.a);

}
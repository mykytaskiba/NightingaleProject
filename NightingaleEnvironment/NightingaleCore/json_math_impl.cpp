#include "color.h"
#include "vector.h"

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


void to_json(nlohmann::json& json, const Vector2& vec)
{
	json["x"] = vec[0];
	json["y"] = vec[1];
}

void from_json(nlohmann::json const& json, Vector2& vec)
{
	json.at("x").get_to(vec[0]);
	json.at("y").get_to(vec[1]);
}

void to_json(nlohmann::json& json, const Vector3& vec)
{
	json["x"] = vec[0];
	json["y"] = vec[1];
	json["z"] = vec[2];
}

void from_json(nlohmann::json const& json, Vector3& vec)
{
	/*
	if (json.contains("x")) {
		vec[0] = json["x"].get<float>();
	}*/
	json.at("x").get_to(vec[0]);
	json.at("y").get_to(vec[1]);
	json.at("z").get_to(vec[2]);
}

void to_json(nlohmann::json& json, const Vector4& vec)
{
	json["x"] = vec[0];
	json["y"] = vec[1];
	json["z"] = vec[2];
	json["w"] = vec[3];
}

void from_json(nlohmann::json const& json, Vector4& vec)
{
	json.at("x").get_to(vec[0]);
	json.at("y").get_to(vec[1]);
	json.at("z").get_to(vec[2]);
	json.at("w").get_to(vec[3]);
}

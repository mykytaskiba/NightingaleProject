#include "pch.h"
#include "ngjson.h"
#include "ngmath.h"

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

void to_json(nlohmann::json& json, const Transform& transform)
{
	json["position"] = transform.position;
	json["rotation"] = transform.rotation;
	json["scale"] = transform.scale;
}

void from_json(nlohmann::json const& json, Transform& transform)
{
	json.at("position").get_to(transform.position);
	json.at("rotation").get_to(transform.rotation);
	json.at("scale").get_to(transform.scale);
}

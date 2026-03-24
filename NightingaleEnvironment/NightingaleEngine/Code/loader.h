#pragma once
#include "defines.h"
#include "assimp/Importer.hpp"
#include "ngmath.h"
#include <filesystem>
#include "bitwise_enum.hpp"
#include "json.hpp"

class Mesh;
class Skeleton;
class Animation;

enum class FileCreationFlags : uint {
    None = 0,
    CreateDirectory = 1 << 0,
    Overwrite = 1 << 1,
    JSONSingleLine = 1 << 2,
};
ENABLE_ENUM_BITWISE_OPERATORS(FileCreationFlags);

class Loader{
private:

    static constexpr unsigned int s_jsonIndent{ 4u };

public:

    static Mesh* fbxSingleMesh(string const& path, float importScale);
    static Skeleton* fbxSkeleton(string const& path, float importScale);
    static Animation* fbxAnimation(string const& path);


    static bool fileExists(std::filesystem::path const& path);

    static bool parseJSON(std::string const& text, nlohmann::json& json);
    static bool readFile(std::filesystem::path const& path, std::string& out);
    static bool readFile(std::filesystem::path const& path, nlohmann::json& out);
    static bool readFile(std::filesystem::path const& path, std::vector<std::string>& out);

    static bool createDirectories(std::filesystem::path const& path);
    static bool saveFile(std::filesystem::path const& path, std::string const& contents, FileCreationFlags flags = FileCreationFlags::CreateDirectory | FileCreationFlags::Overwrite);
    static bool saveFile(std::filesystem::path const& path, nlohmann::json const& json, FileCreationFlags flags = FileCreationFlags::CreateDirectory | FileCreationFlags::Overwrite);
    

    static bool loadTexture(string const& path);

    //ASSIMP Helpers
    static Matrix4x4 fromAIMatrix(aiMatrix4x4 const&);
    static Vector3 fromAIVector3(aiVector3f const&);
    static Quaternion fromAIQuat(aiQuaternion const&);
    static aiMatrix4x4 parentTransformation(aiNode const*);

private:

    static void allNodes(aiNode const* root, vector<aiNode const*>& outList);
    static void findMeshedNodes(aiNode const* root, vector<aiNode const*>& outList);
    static aiNode const* findFirstMeshedNode(aiNode const* root);


};
#pragma once
#include "defines.h"
#include "assimp/Importer.hpp"
#include "ngmath.h"
#include <filesystem>
#include "bitwise_enum.hpp"
#include "json.hpp"
#include "mesh_data.h"

class Mesh;
class Skeleton;
class Animation;

enum class FileCreationFlags : unsigned int {
    None = 0,
    CreateDirectory = 1 << 0,
    Overwrite = 1 << 1,
    JSONSingleLine = 1 << 2,
};
ENABLE_ENUM_BITWISE_OPERATORS(FileCreationFlags);

enum class ImageLoadingFlags : unsigned int {
    None = 0,
    ForceChannel_R = 1 << 0,
    ForceChannel_RG = 1 << 1,
    ForceChannel_RGB = 1 << 2,
    ForceChannel_RGBA = 1 << 3,
};
ENABLE_ENUM_BITWISE_OPERATORS(ImageLoadingFlags);

enum class MeshLoadingFlags : unsigned int {
    None = 0,
};
ENABLE_ENUM_BITWISE_OPERATORS(MeshLoadingFlags);

class Loader{
private:

    static constexpr unsigned int s_jsonIndent{ 4u };

public:

    static Mesh* fbxSingleMesh(std::string const& path, float importScale);
    static Skeleton* fbxSkeleton(std::string const& path, float importScale);
    static Animation* fbxAnimation(std::string const& path);


    static bool fileExists(std::filesystem::path const& path);
    static bool fileIsDirectory(std::filesystem::path const& path);
    static bool fileIsPlainFile(std::filesystem::path const& path);

    static bool parseJSON(std::string const& text, nlohmann::json& json);
    static bool readFile(std::filesystem::path const& path, std::string& out);
    static bool readFile(std::filesystem::path const& path, nlohmann::json& out);
    static bool readFile(std::filesystem::path const& path, std::vector<std::string>& out);

    static bool createDirectories(std::filesystem::path const& path);
    static bool saveFile(std::filesystem::path const& path, std::string const& contents, FileCreationFlags flags = FileCreationFlags::CreateDirectory | FileCreationFlags::Overwrite);
    static bool saveFile(std::filesystem::path const& path, nlohmann::json const& json, FileCreationFlags flags = FileCreationFlags::CreateDirectory | FileCreationFlags::Overwrite);
    
    static bool loadImage(std::filesystem::path const& path, ImageLoadingFlags flags = ImageLoadingFlags::None);

    static bool readMeshData(std::filesystem::path const& path, std::vector<MeshData*>& outData);

    static bool buildMeshData(aiMesh const& aiMesh, MeshData& outMesh);


    //ASSIMP Helpers
    static Matrix4x4 fromAIMatrix(aiMatrix4x4 const&);
    static Vector3 fromAIVector3(aiVector3f const&);
    static Quaternion fromAIQuat(aiQuaternion const&);
    static aiMatrix4x4 parentTransformation(aiNode const*);

private:

    static void allNodes(aiNode const* root, std::vector<aiNode const*>& outList);
    static void findMeshedNodes(aiNode const* root, std::vector<aiNode const*>& outList);
    static aiNode const* findFirstMeshedNode(aiNode const* root);


};



//=============================
//Builder

enum eVertexDataFlags {
    POSITION = 1 << 0,
    NORMAL = 1 << 1,
    TEXUV = 1 << 2,
    BONEINDICIES = 1 << 3,
    BONEWEIGHTS = 1 << 4
};

using VertexDataFormat = unsigned int;

class SkeletonBuilder;

class MeshBuilder {

public:
    MeshBuilder(aiMesh const*, aiMatrix4x4 const&);
    MeshBuilder(aiMesh const*, SkeletonBuilder const& skeletonBuilder);

    bool isSuccessful() const;
    Mesh* getMesh() const;

private:

    struct BoneWeight {
        uint indicies[4] = { 0,0,0,0 };
        float weights[4] = { 0.0f, 0.0f, 0.0f,0.0f };

        void addWeight(uint index, float weight);
    };

    void loadBoneWeights(aiMesh const* pMesh, SkeletonBuilder const& skeletonBuilder);

    bool m_bResultSuccess{ false };
    Mesh* m_pMesh{ nullptr };

    uint m_vertexCount{ 0 };
    std::vector<float> m_vertexData{};

    uint m_faceCount{ 0 };
    std::vector<uint> m_faceData{};

    std::vector<BoneWeight> m_vertexBoneData;

    VertexDataFormat m_vertexDataFormat{ 0 };
    uint m_dataSizePerVertex{ 0 };


    bool m_bPrepared{ false };
    bool m_bFinalized{ false };


    void calculateDataSizePerVertex();


    void prepare(VertexDataFormat vertexData, uint vertexCount, uint triangleCount);
    void finalize();

    void pushFloat(float f);
    void pushUInt(uint i);

    void createFaces(aiMesh const* pMeshAI);
    void pushFace(uint i1, uint i2, uint i3);

};

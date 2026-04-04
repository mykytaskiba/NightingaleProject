#include "pch.h"
#include "loader.h"

#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "mesh.h"
#include "defines.h"
#include "skeleton.h"
#include "animation.h"
#include "skeleton_builder.h"
#include "animation_builder.h"
#include "ngmath.h"
#include "stb_image.h"



Mesh* Loader::fbxSingleMesh(std::string const& path, float importScale)
{
    Assimp::Importer importer;

    

    aiScene const* scene = importer.ReadFile(path, aiProcess_Triangulate);
    assert(scene != nullptr);

    int numMeshes = scene->mNumMeshes;
    assert(numMeshes == 1);
    
    aiMesh const* mesh = scene->mMeshes[0];
    assert(mesh != nullptr);

    assert(scene->mRootNode != nullptr);
    
    aiNode const* node = findFirstMeshedNode(scene->mRootNode);
    assert(node != nullptr);

    
    aiMatrix4x4 modelScale;
    modelScale.a1 = modelScale.b2 = modelScale.c3 = importScale;
    aiMatrix4x4 modelTransform = parentTransformation(node) * modelScale;

    MeshBuilder builder(mesh, modelTransform);
    if (!builder.isSuccessful()) {
        assert(0);
        return nullptr;
    }

    return builder.getMesh();
}

//Assumption: one skeleton and all meshes are rigged to that skeleton
Skeleton* Loader::fbxSkeleton(std::string const& path, float importScale)
{
    Assimp::Importer importer;

    aiScene const* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights);
    assert(scene != nullptr);

    aiMatrix4x4 modelScale;
    modelScale.a1 = modelScale.b2 = modelScale.c3 = importScale;

    SkeletonBuilder builder(scene, modelScale);

    assert(builder.isSuccessful());
    return builder.getSkeleton();
    
}

Animation* Loader::fbxAnimation(std::string const& path)
{
    Assimp::Importer importer;

    aiScene const* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights);
    assert(scene != nullptr);

    AnimationBuilder builder(scene);

    assert(builder.isSuccessful());
    return builder.getAnimation();
}


bool Loader::fileExists(std::filesystem::path const& path)
{
    return (std::filesystem::exists(path));
}

bool Loader::fileIsPlainFile(std::filesystem::path const& path)
{
    return std::filesystem::is_regular_file(path);
}

bool Loader::fileIsDirectory(std::filesystem::path const& path)
{
    return std::filesystem::is_directory(path);
}


bool Loader::parseJSON(std::string const& text, nlohmann::json& json)
{
    try {
        nlohmann::json temp = nlohmann::json::parse(text);
        json = std::move(temp);
    }
    catch (nlohmann::json::exception const&) {
        return false;
    }

    return true;
}

bool Loader::readFile(std::filesystem::path const& path, std::string& out)
{
    std::ifstream fileStream(path, std::ios::binary);
    if (!fileStream) {
        return false;
    }

    try {
        fileStream.seekg(0, std::ios::end);
        std::string temp(fileStream.tellg(), '\0');
        fileStream.seekg(0, std::ios::beg);

        fileStream.read(temp.data(), temp.size());

        out = std::move(temp);
    }
    catch (...) {
        return false;
    }

    return true;
}

bool Loader::readFile(std::filesystem::path const& path, nlohmann::json& out)
{
    std::string fileText;
    if (!readFile(path, fileText)) {
        return false;
    }
    return parseJSON(fileText, out);
}

bool Loader::readFile(const std::filesystem::path& path, std::vector<std::string>& out)
{
    std::string content;
    if (!readFile(path, content)) {
        return false;
    }

    std::vector<std::string> temp;
    std::istringstream ss(content);
    std::string line;

    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') { //this could be a helper
            line.pop_back();
        }
        temp.push_back(std::move(line));
    }

    out = std::move(temp);
    return true;
}


bool Loader::saveFile(std::filesystem::path const& path, std::string const& contents, FileCreationFlags flags)
{
    std::filesystem::path absolutePath = std::filesystem::absolute(path);

    bool bFileExists = std::filesystem::exists(absolutePath);

    bool bIsDirectory = std::filesystem::is_directory(absolutePath) && bFileExists;
    if (bIsDirectory) {
        return false;
    }

    std::filesystem::path directoryPath = absolutePath.parent_path();
    
    bool bDirectoryExists = std::filesystem::exists(directoryPath);

    if (bFileExists) {
        if ((flags & FileCreationFlags::Overwrite) == FileCreationFlags::None) {
            return false;
        }
    }
    if (!bDirectoryExists) {
        if ((flags & FileCreationFlags::CreateDirectory) == FileCreationFlags::None) {
            return false;
        }
    }

    if (!bDirectoryExists) {
        if (!std::filesystem::create_directories(directoryPath)) {
            return false;
        }
    }

    std::ofstream fileStream(absolutePath);

    if (!fileStream) {
        //Failed to open or create file
        return false;
    }

    fileStream << contents;
    bool bWriteSuccess = fileStream.good();
    fileStream.close();
    return bWriteSuccess;
}

bool Loader::saveFile(std::filesystem::path const& path, nlohmann::json const& json, FileCreationFlags flags)
{
    if ((flags & FileCreationFlags::JSONSingleLine) == FileCreationFlags::None) {
        return saveFile(path, json.dump(s_jsonIndent), flags);
    }
    else {
        return saveFile(path, to_string(json), flags);
    }
}

bool Loader::loadImage(std::filesystem::path const& path, ImageLoadingFlags flags)
{
    assert(false); // NOT FINISHED!

    int width{ 0 };
    int height{ 0 };
    int channels{ 0 };

    int desiredChannels = 0;
    if ((flags & ImageLoadingFlags::ForceChannel_R) == ImageLoadingFlags::ForceChannel_R) {
        desiredChannels = 1;
    }
    if ((flags & ImageLoadingFlags::ForceChannel_RG) == ImageLoadingFlags::ForceChannel_RG) {
        desiredChannels = 2;
    }
    if ((flags & ImageLoadingFlags::ForceChannel_RGB) == ImageLoadingFlags::ForceChannel_RGB) {
        desiredChannels = 3;
    }
    if ((flags & ImageLoadingFlags::ForceChannel_RGBA) == ImageLoadingFlags::ForceChannel_RGBA) {
        desiredChannels = 4;
    }

    stbi_set_flip_vertically_on_load_thread(true);
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.string().c_str(), &width, &height, &channels, desiredChannels);
    //std::string failure_reason{ stbi_failure_reason() };
    return false;
}

bool Loader::readMeshData(std::filesystem::path const& path, std::vector<MeshData*>& outVectorData)
{
    if (!fileExists(path) && !fileIsPlainFile(path)) {
        return false;
    }

    Assimp::Importer importer;

    aiScene const* pScene = importer.ReadFile(path.string(), aiProcess_Triangulate);
    if (pScene == nullptr) {
        assert(false);
        return false;
    }

    int numMeshes = pScene->mNumMeshes;

    for (int i{ 0 }; i < numMeshes; ++i) {
        aiMesh const* pMesh = pScene->mMeshes[i];
        if (pMesh == nullptr) {
            assert(false); 
            return false; //valid invalid in this, should probably exit
        }

        outVectorData.push_back(new MeshData());
        if (outVectorData.back() == nullptr) {
            assert(false);
            return false; //we probably ran out of memory
        }
        MeshData& meshData = *outVectorData.back();
        bool bResult = buildMeshData(*pMesh, meshData);
        if (!bResult) return false; //something went wrong building the mesh?
    }

    return true;
}

bool Loader::buildMeshData(aiMesh const& aiMesh, MeshData& outMesh)
{
    if (outMesh.hasData()) {
        return false;
    }

    size_t numVerticies = aiMesh.mNumVertices;
    if (numVerticies == 0) {
        return false;
    }

    size_t numFaces = aiMesh.mNumFaces;
    if (numFaces == 0) {
        return false;
    }

    outMesh.addAttribute(MeshData::Attribute{ "position", MeshData::PrimitiveType::FLOAT, 3u });
    outMesh.addAttribute(MeshData::Attribute{ "normal", MeshData::PrimitiveType::FLOAT, 3u });
    outMesh.addAttribute(MeshData::Attribute{ "texuv", MeshData::PrimitiveType::FLOAT, 2u });
    outMesh.prepareVertexData(numVerticies);

    for (int loop = 0; loop < numVerticies; loop++) {

        aiVector3D const& vertex = aiMesh.mVertices[loop];
        aiVector3D const& normal = aiMesh.mNormals[loop];
        aiVector3D const& texCoord = aiMesh.mTextureCoords[0][loop];

        outMesh.pushVertexData(vertex.x);
        outMesh.pushVertexData(vertex.y);
        outMesh.pushVertexData(vertex.z);

        outMesh.pushVertexData(normal.x);
        outMesh.pushVertexData(normal.y);
        outMesh.pushVertexData(normal.z);

        outMesh.pushVertexData(texCoord.x);
        outMesh.pushVertexData(texCoord.z);
    }

    outMesh.prepareFaceData(numFaces);

    for (int loop = 0; loop < numFaces; loop++) {
        aiFace const& face = aiMesh.mFaces[loop];

        if (face.mNumIndices != 3) {
            assert(false); //assume we always dealing with triangles
            return false;
        }

        outMesh.pushFace(MeshData::Triangle{ face.mIndices[0],face.mIndices[1],face.mIndices[2] });
    }

    return true;
}

bool Loader::createDirectories(std::filesystem::path const& path)
{
    return false;
}



Matrix4x4 Loader::fromAIMatrix(aiMatrix4x4 const& aiMat)
{
    Matrix4x4 outMatrix;

    outMatrix[0][0] = aiMat.a1;
    outMatrix[0][1] = aiMat.a2;
    outMatrix[0][2] = aiMat.a3;
    outMatrix[0][3] = aiMat.a4;

    outMatrix[1][0] = aiMat.b1;
    outMatrix[1][1] = aiMat.b2;
    outMatrix[1][2] = aiMat.b3;
    outMatrix[1][3] = aiMat.b4;

    outMatrix[2][0] = aiMat.c1;
    outMatrix[2][1] = aiMat.c2;
    outMatrix[2][3] = aiMat.c4;
    outMatrix[2][2] = aiMat.c3;

    outMatrix[3][0] = aiMat.d1;
    outMatrix[3][1] = aiMat.d2;
    outMatrix[3][2] = aiMat.d3;
    outMatrix[3][3] = aiMat.d4;
    return outMatrix;
}

Vector3 Loader::fromAIVector3(aiVector3f const& vec)
{
    return Vector3(vec.x, vec.y, vec.z);
}

Quaternion Loader::fromAIQuat(aiQuaternion const& quat)
{
    return Quaternion(quat.x, quat.y, quat.z, quat.w);
}

aiMatrix4x4 Loader::parentTransformation(aiNode const* node)
{
    assert(node != nullptr);

    aiMatrix4x4 transform;

    aiNode const* nodeIt = node;
    while (nodeIt != nullptr) {

        transform = nodeIt->mTransformation * transform;
        nodeIt = nodeIt->mParent;
    }
    return transform;
}

void Loader::allNodes(aiNode const* root, std::vector<aiNode const*>& outList)
{
    outList.push_back(root);

    for (uint i = 0; i < root->mNumChildren; ++i) {
        allNodes(root->mChildren[i], outList);
    }
}

void Loader::findMeshedNodes(aiNode const* root, std::vector<aiNode const*>& outList)
{
    if (root->mNumMeshes > 0) {
        outList.push_back(root);
    }

    for (uint i = 0; i < root->mNumChildren; ++i) {
        findMeshedNodes(root->mChildren[i], outList);
    }
}

aiNode const* Loader::findFirstMeshedNode(aiNode const* root)
{
    if (root->mNumMeshes != 0) {
        return root;
    }
    aiNode const* result;
    for (uint i = 0; i < root->mNumChildren; ++i) {
        result = findFirstMeshedNode(root->mChildren[i]);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

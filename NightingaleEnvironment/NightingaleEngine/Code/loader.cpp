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



Mesh* Loader::fbxSingleMesh(string const& path, float importScale)
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
Skeleton* Loader::fbxSkeleton(string const& path, float importScale)
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

Animation* Loader::fbxAnimation(string const& path)
{
    Assimp::Importer importer;

    aiScene const* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights);
    assert(scene != nullptr);

    AnimationBuilder builder(scene);

    assert(builder.isSuccessful());
    return builder.getAnimation();
}


bool Loader::file_exists(string const& pathIn)
{
    filesystem::path path = pathIn;
    return (filesystem::exists(path));
}

string Loader::read_file_contents(string const& path)
{
    ifstream file;
    file.open(path);

    if (!file.is_open()) {
        assert(0);
        return "";
    }

    string fileContents;
    stringstream buffer;

    buffer << file.rdbuf();
    fileContents = buffer.str();
    return fileContents;
}

vector<string> Loader::read_file_contents_by_line(string const& path)
{
    string file_contents = read_file_contents(path);

    std::vector<std::string> lines;
    std::stringstream ss(file_contents);
    std::string line;

    while (std::getline(ss, line, '\n')) {
        lines.push_back(line);
    }
    return lines;
}

bool Loader::loadTexture(string const& path)
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

void Loader::allNodes(aiNode const* root, vector<aiNode const*>& outList)
{
    outList.push_back(root);

    for (uint i = 0; i < root->mNumChildren; ++i) {
        allNodes(root->mChildren[i], outList);
    }
}

void Loader::findMeshedNodes(aiNode const* root, vector<aiNode const*>& outList)
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

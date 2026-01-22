#pragma once
#include "defines.h"
#include "assimp/Importer.hpp"
#include "ngmath.h"

class Mesh;
class Skeleton;
class Animation;

struct Loader {
private:

public:
    string identifier;

    static Mesh* fbxSingleMesh(string const& path, float importScale);
    static Skeleton* fbxSkeleton(string const& path, float importScale);
    static Animation* fbxAnimation(string const& path);

    static bool file_exists(string const& path);

    static string read_file_contents(string const& path);

    static vector<string> read_file_contents_by_line(string const& path);

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
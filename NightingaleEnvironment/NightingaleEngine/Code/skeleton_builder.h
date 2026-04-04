#pragma once
#include <assimp/Importer.hpp>
#include "defines.h"

class Skeleton;

class SkeletonBuilder {
private:
    struct BuilderNode;
    struct BuilderBone;
public:
    SkeletonBuilder(aiScene const*, aiMatrix4x4 const&);
    bool isSuccessful() const;
    Skeleton* getSkeleton() const;

    uint getBoneIndex(aiBone const* pBone) const;

private:
    uint m_numNodes{ 0 };
    uint m_numBones{ 0 };
    std::vector<BuilderNode> m_nodes{};
    std::map<std::string, uint> m_nodeNameMap{};

    std::map<uint, BuilderBone> m_boneMap{};
    std::map<std::string, uint> m_boneNameMap{};

    bool m_bSuccess{ false };
    Skeleton* m_pSkeleton{ nullptr };

    //initial call for the recursive skeleton node building
    void buildSkeletonNodes(aiScene const* pScene);
    //recursive get nodes
    void buildSkeletonNodesRecurse(aiNode const* pNode, uint parent);

    void buildSkeletonBones(aiScene const* pScene);

    void buildMeshes(aiScene const* pScene);


    //a private struct for the builder to use for temporary node information
    struct BuilderNode {
        uint parent{ 0 };
        aiNode const* pNode;

    };

    struct BuilderBone {
        aiBone const* pBone;
        uint linkedNode;
    };

};
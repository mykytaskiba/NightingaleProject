#include "pch.h"
#include "skeleton_builder.h"
#include "defines.h"
#include "skeleton.h"
#include "loader.h"
#include "mesh.h"

SkeletonBuilder::SkeletonBuilder(aiScene const* pScene, aiMatrix4x4 const& importTransform)
{
    assert(pScene != nullptr);

    assert(pScene->mNumMeshes > 0);
    aiMesh const* pMesh = pScene->mMeshes[0];

    assert(pMesh->mNumBones > 0);
    aiBone const* pBone = pMesh->mBones[0];

    assert(pBone->mArmature != nullptr);
    aiNode const* pArmature = pBone->mArmature;

    m_pSkeleton = new Skeleton();
    //Load the node hierarchy
    buildSkeletonNodes(pScene);
    buildSkeletonBones(pScene);


    m_pSkeleton->m_nodes.reserve(m_nodes.size());
    m_pSkeleton->m_bindPose.reserve(m_nodes.size());
    for (auto it = m_nodes.begin(); it != m_nodes.end(); ++it) {
        Skeleton::SkelNode node;
        node.parent = (*it).parent;

        if ((*it).pNode != nullptr) {
            node.transform = Loader::fromAIMatrix((*it).pNode->mTransformation);
        }
        else {
            //special case for corenode
            node.transform.make_identity();
        }
        
        m_pSkeleton->m_nodes.push_back(node);
        m_pSkeleton->m_bindPose.push_back(node.transform);//create the bind pose at the same time
    }

    m_pSkeleton->m_bones.reserve(m_boneMap.size());
    for (auto it = m_boneMap.begin(); it != m_boneMap.end(); ++it) {
        Skeleton::SkelBone bone;
        bone.linkedNode = (*it).first;
        bone.offsetMatrix = Loader::fromAIMatrix((*it).second.pBone->mOffsetMatrix);

        m_pSkeleton->m_bones.push_back(bone);
    }
    
    m_pSkeleton->m_nodeTransforms.resize(m_numNodes);
    m_pSkeleton->m_boneTransforms.resize(m_numBones);

    aiMatrix4x4 parentTransform = Loader::parentTransformation(pArmature);

    m_pSkeleton->setParentTransform(Loader::fromAIMatrix(importTransform * parentTransform));
    m_pSkeleton->m_transform;
    m_pSkeleton->m_transform.make_identity();

    m_pSkeleton->calcCurrentTransform();
    m_pSkeleton->calcBoneMatricies();


    
    buildMeshes(pScene);

    m_bSuccess = true;
}

bool SkeletonBuilder::isSuccessful() const
{
    return m_bSuccess;
}

Skeleton* SkeletonBuilder::getSkeleton() const
{
    return m_pSkeleton;
}

uint SkeletonBuilder::getBoneIndex(aiBone const* pBone) const
{
    auto it = m_boneNameMap.find(std::string(pBone->mName.C_Str()));
    if (it == m_boneNameMap.end()) {
        assert(0);
        return 0;
    }
    return (*it).second;
}

void SkeletonBuilder::buildSkeletonNodes(aiScene const* pScene)
{
    //a fake corenode to act as a core for all other nodes
    //BuilderNode node;
    //node.pNode = nullptr;
    //node.parent = 0;

    //m_nodes.push_back(node);
    //m_nodeNameMap["CORENODE"] = 0;

    buildSkeletonNodesRecurse(pScene->mRootNode, 0);
    /*
    for (uint i = 0; i < pScene->mNumMeshes; ++i) {
        aiMesh const* pMesh = pScene->mMeshes[i];

        assert(pMesh->mNumBones > 0);
        aiNode const* pArmature = pMesh->mBones[0]->mArmature;
        
        bool bAlreadyInHierarchy = m_nodeNameMap.find(string(pArmature->mName.C_Str())) != m_nodeNameMap.end();
        if (bAlreadyInHierarchy) continue;

        buildSkeletonNodesRecurse(pArmature, 0);
    }*/

    //first node will have a parent of 0, 
    //since it is the first node we hard code it to not have a parent when calculating various matricies
    
    m_pSkeleton->m_nodeNameMap = m_nodeNameMap;

    m_numNodes = m_nodes.size();
}

void SkeletonBuilder::buildSkeletonNodesRecurse(aiNode const* pNode, uint parent)
{
    assert(pNode != nullptr);

    BuilderNode node; 
    node.pNode = pNode;
    node.parent = parent;     
    m_nodes.push_back(node);

    uint thisNodeIndex = m_nodes.size() - 1; //since we just added, this node index is size-1
 
    m_nodeNameMap[std::string(pNode->mName.C_Str())] = thisNodeIndex;

    for (uint i = 0; i < pNode->mNumChildren; ++i) {
        buildSkeletonNodesRecurse(pNode->mChildren[i], thisNodeIndex);
    }
}

void SkeletonBuilder::buildSkeletonBones(aiScene const* pScene)
{
    for (uint meshID = 0; meshID < pScene->mNumMeshes; ++meshID) {
        aiMesh const* pMesh = pScene->mMeshes[meshID];
        for (uint i = 0; i < pMesh->mNumBones; ++i) {
            aiBone const* pBone = pMesh->mBones[i];
            uint nodeIndex = m_nodeNameMap[std::string(pBone->mName.C_Str())];

            if (m_boneMap.find(nodeIndex) == m_boneMap.end()) {
                m_boneMap[nodeIndex] = BuilderBone();
                m_boneMap[nodeIndex].linkedNode = nodeIndex;
                m_boneMap[nodeIndex].pBone = pBone;
            }

        }
    }

    uint boneIndex = 0;
    for (auto it = m_boneMap.begin(); it != m_boneMap.end(); ++it) {
        m_boneNameMap[std::string ((*it).second.pBone->mName.C_Str())] = boneIndex;
        ++boneIndex;
    }

    m_numBones = m_boneMap.size();
}

void SkeletonBuilder::buildMeshes(aiScene const* pScene)
{
    for (uint i = 0; i < pScene->mNumMeshes; ++i) {
        aiMesh const* pMesh = pScene->mMeshes[i];
        MeshBuilder builder(pMesh, *this);
        assert(builder.isSuccessful());
        Mesh* mesh = builder.getMesh();
        m_pSkeleton->m_meshes.push_back(mesh);
    }
}

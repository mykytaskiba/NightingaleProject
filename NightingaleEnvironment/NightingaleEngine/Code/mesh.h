#pragma once
#include "defines.h"

class Mesh {

    friend class MeshBuilder;

private:
    uint m_VAO;
    uint m_triangleCount;
    //TO DO: Make this private
    Mesh(uint VAO, uint triangleCount) : m_VAO(VAO), m_triangleCount(triangleCount) { }

public:

    void Draw();

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

using VertexDataFormat = uint;

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
    vector<float> m_vertexData{};

    uint m_faceCount{ 0 };
    vector<uint> m_faceData{};

    vector<BoneWeight> m_vertexBoneData;

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

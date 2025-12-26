#include "pch.h"
#include "mesh.h"
#include "skeleton_builder.h"

void Mesh::Draw()
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_triangleCount, GL_UNSIGNED_INT, 0);
}

void MeshBuilder::finalize()
{
    assert(m_bPrepared && !m_bFinalized);
    m_bFinalized = true;

    assert(m_faceData.size() == (size_t)m_faceCount * 3);
    assert(m_vertexData.size() == ((size_t)m_vertexCount * (size_t)m_dataSizePerVertex));
    

    //create buffers
    uint VBO;
    glGenBuffers(1, &VBO);
    uint EBO;
    glGenBuffers(1, &EBO);
    uint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    //copy vertex array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertexData.size() * sizeof(float), m_vertexData.data(), GL_STATIC_DRAW);

    uint stride = m_dataSizePerVertex * sizeof(float);
    
    uint currentOffset = 0;
    //vertex attributes
    if ((m_vertexDataFormat & POSITION) != 0) {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(currentOffset * sizeof(float)));
        currentOffset += 3;
    }
    if ((m_vertexDataFormat & NORMAL) != 0) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(currentOffset * sizeof(float)));
        currentOffset += 3;
    }
    if ((m_vertexDataFormat & TEXUV) != 0) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(currentOffset * sizeof(float)));
        currentOffset += 2;
    }
    if ((m_vertexDataFormat & BONEINDICIES) != 0) {
        glEnableVertexAttribArray(3);
        glVertexAttribIPointer(3, 4, GL_INT, stride, (void*)(currentOffset * sizeof(float)));
        currentOffset += 4;
    }
    if ((m_vertexDataFormat & BONEWEIGHTS) != 0) {
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (void*)(currentOffset * sizeof(float)));
        currentOffset += 4;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_faceData.size() * sizeof(uint), m_faceData.data(), GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_pMesh = new Mesh(VAO, m_faceCount * 3);
    m_bResultSuccess = true;
}

MeshBuilder::MeshBuilder(aiMesh const* pMeshAI, aiMatrix4x4 const& transform)
{
    size_t numVerticies = pMeshAI->mNumVertices;
    assert(numVerticies > 0);

    assert(pMeshAI->mVertices != nullptr);
    assert(pMeshAI->mNormals != nullptr);
    assert(pMeshAI->mNumUVComponents != nullptr);
    //assumption, texCoord is stored in the 0s uv component
    //assumption, texCoord.z is ignored
    assert(pMeshAI->mNumUVComponents[0] == 2);
    assert(pMeshAI->mTextureCoords != nullptr);


    aiVector3D scaling;
    aiQuaternion rotation;
    aiVector3D translation;
    transform.Decompose(scaling, rotation, translation); // need just the rotation of the transform

    aiMatrix3x3 rotationTransform = rotation.GetMatrix();


    size_t numFaces = pMeshAI->mNumFaces;
    assert(numFaces > 0);
    assert(pMeshAI->mFaces != nullptr);

    VertexDataFormat vertexDataFormat = POSITION | NORMAL | TEXUV;
    
    prepare(vertexDataFormat, numVerticies, numFaces);

    for (int i = 0; i < numVerticies; i++) {

        aiVector3D vertex = transform * pMeshAI->mVertices[i];
        aiVector3D normal = rotationTransform * pMeshAI->mNormals[i];
        aiVector3D const& texCoord = pMeshAI->mTextureCoords[0][i];

        pushFloat(vertex.x);
        pushFloat(vertex.y);
        pushFloat(vertex.z);

        pushFloat(normal.x);
        pushFloat(normal.y);
        pushFloat(normal.z);

        pushFloat(texCoord.x);
        pushFloat(texCoord.y);
    }

    createFaces(pMeshAI);


    finalize();
}

MeshBuilder::MeshBuilder(aiMesh const* pMeshAI, SkeletonBuilder const& skeletonBuilder)
{
    size_t numVerticies = pMeshAI->mNumVertices;
    assert(numVerticies > 0);

    assert(pMeshAI->mVertices != nullptr);
    assert(pMeshAI->mNormals != nullptr);
    assert(pMeshAI->mNumUVComponents != nullptr);
    //assumption, texCoord is stored in the 0s uv component
    //assumption, texCoord.z is ignored
    assert(pMeshAI->mNumUVComponents[0] == 2);
    assert(pMeshAI->mTextureCoords != nullptr);

    aiMatrix4x4 transform;

    aiVector3D scaling;
    aiQuaternion rotation;
    aiVector3D translation;
    transform.Decompose(scaling, rotation, translation); // need just the rotation of the transform

    aiMatrix3x3 rotationTransform = rotation.GetMatrix();


    size_t numFaces = pMeshAI->mNumFaces;
    assert(numFaces > 0);
    assert(pMeshAI->mFaces != nullptr);

    VertexDataFormat vertexDataFormat = POSITION | NORMAL | TEXUV;

    vertexDataFormat |= BONEINDICIES | BONEWEIGHTS;
    

    prepare(vertexDataFormat, numVerticies, numFaces);

    loadBoneWeights(pMeshAI, skeletonBuilder);
    assert(m_vertexBoneData.size() == m_vertexCount);    


    for (int i = 0; i < numVerticies; i++) {

        aiVector3D vertex = transform * pMeshAI->mVertices[i];
        aiVector3D normal = rotationTransform * pMeshAI->mNormals[i];
        aiVector3D const& texCoord = pMeshAI->mTextureCoords[0][i];

        pushFloat(vertex.x);
        pushFloat(vertex.y);
        pushFloat(vertex.z);

        pushFloat(normal.x);
        pushFloat(normal.y);
        pushFloat(normal.z);

        pushFloat(texCoord.x);
        pushFloat(texCoord.y);

        //bone indicies
        pushUInt(m_vertexBoneData[i].indicies[0]);
        pushUInt(m_vertexBoneData[i].indicies[1]);
        pushUInt(m_vertexBoneData[i].indicies[2]);
        pushUInt(m_vertexBoneData[i].indicies[3]);
        //bone weights
        pushFloat(m_vertexBoneData[i].weights[0]);
        pushFloat(m_vertexBoneData[i].weights[1]);
        pushFloat(m_vertexBoneData[i].weights[2]);
        pushFloat(m_vertexBoneData[i].weights[3]);

    }

    createFaces(pMeshAI);

    finalize();
}

bool MeshBuilder::isSuccessful() const
{
    return m_bResultSuccess;
}

Mesh* MeshBuilder::getMesh() const
{
    assert(m_bResultSuccess);
    return m_pMesh;
}

void MeshBuilder::loadBoneWeights(aiMesh const* pMesh, SkeletonBuilder const& skeletonBuilder)
{
    assert(pMesh != nullptr);

    m_vertexBoneData.resize(m_vertexCount);

    for (uint i = 0; i < pMesh->mNumBones; ++i) {
        aiBone const* bone = pMesh->mBones[i];
        uint boneIndex = skeletonBuilder.getBoneIndex(bone);
        for (uint j = 0; j < bone->mNumWeights; ++j) {
            aiVertexWeight const& vWeight = bone->mWeights[j];
            m_vertexBoneData[vWeight.mVertexId].addWeight(boneIndex, vWeight.mWeight);
        }
    }
}

void MeshBuilder::calculateDataSizePerVertex()
{
    m_dataSizePerVertex = 0;

    //data size in terms of floats
    constexpr uint vec3_size = 3;
    constexpr uint vec2_size = 2;
    constexpr uint vec4i_size = 4;
    constexpr uint vec4_size = 4;

    if ((m_vertexDataFormat & eVertexDataFlags::POSITION) != 0) 
        m_dataSizePerVertex += vec3_size;
    
    if ((m_vertexDataFormat & eVertexDataFlags::NORMAL) != 0)
        m_dataSizePerVertex += vec3_size;

    if ((m_vertexDataFormat & eVertexDataFlags::TEXUV) != 0)
        m_dataSizePerVertex += vec2_size;

    if ((m_vertexDataFormat & eVertexDataFlags::BONEINDICIES) != 0)
        m_dataSizePerVertex += vec4i_size;

    if ((m_vertexDataFormat & eVertexDataFlags::BONEWEIGHTS) != 0)
        m_dataSizePerVertex += vec4_size;
}

void MeshBuilder::prepare(VertexDataFormat vertexDataFormat, uint vertexCount, uint triangleCount)
{
    assert(!m_bPrepared && !m_bFinalized);
    m_bPrepared = true;

    m_vertexDataFormat = vertexDataFormat;
    calculateDataSizePerVertex();

    assert(vertexCount > 0);
    m_vertexCount = vertexCount;
    m_vertexData.reserve((size_t)m_vertexCount * (size_t)m_dataSizePerVertex);

    assert(triangleCount > 0);
    m_faceCount = triangleCount;
    m_faceData.reserve((size_t)triangleCount * 3);

}

void MeshBuilder::pushFloat(float f)
{
    assert(m_bPrepared && !m_bFinalized);
    m_vertexData.push_back(f);
    assert(m_vertexData.size() <= ((size_t)m_vertexCount * (size_t)m_dataSizePerVertex));
}

void MeshBuilder::pushUInt(uint i)
{
    pushFloat(std::bit_cast<float>(i));
}

void MeshBuilder::createFaces(aiMesh const* pMeshAI)
{
    uint numFaces = pMeshAI->mNumFaces;

    for (int i = 0; i < numFaces; i++) {

        aiFace const& face = pMeshAI->mFaces[i];
        assert(face.mNumIndices == 3);

        pushFace(face.mIndices[0], face.mIndices[1], face.mIndices[2]);
    }
}

void MeshBuilder::pushFace(uint i1, uint i2, uint i3)
{
    assert(m_bPrepared && !m_bFinalized);
    m_faceData.push_back(i1);
    m_faceData.push_back(i2);
    m_faceData.push_back(i3);
    assert(m_faceData.size() <= (size_t)m_faceCount * 3);
}

void MeshBuilder::BoneWeight::addWeight(uint index, float weight)
{
    for (uint i = 0; i < 4; ++i) {
        if (weights[i] == 0.0f) {
            weights[i] = weight;
            indicies[i] = index;
            return;
        }
    }
    assert(0);
    return;
}

#include "pch.h"
#include "loader.h"

#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "ngrender.h"
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


//=======================================================================================================

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

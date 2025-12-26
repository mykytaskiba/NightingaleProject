#include "pch.h"
#include "skeleton.h"

//TO DO: REMOVE THESE INCLUDES
#include "render_shader.h"
#include "mesh.h"
#include "animation.h"
#include "graphics_context.h"




void Skeleton::calcCurrentTransform()
{
    //this approach works since the nodes are created in a depth first search. 
    //Thus a parent's transformation is guarranteed to be calculated before any given children
    

    //nasty root node transform trick
    m_nodeTransforms[0] = m_nodes[0].transform * m_transform * m_parentTransform;

    auto it_node = m_nodes.begin();


    uint index = 1;
    ++it_node;

    //go through every non first node
    for (;  it_node != m_nodes.end(); ++it_node) {
        Matrix4x4 const& parent_transform = m_nodeTransforms[(*it_node).parent];
        m_nodeTransforms[index] = parent_transform * m_nodes[index].transform;
        
        ++index;
    }
}

void Skeleton::calcBoneMatricies()
{
    uint index = 0;
    for (auto it_bone = m_bones.begin(); it_bone != m_bones.end(); ++it_bone) {
        uint node_id = (*it_bone).linkedNode;
        m_boneTransforms[index] =  m_nodeTransforms[node_id] * (*it_bone).offsetMatrix;

        ++index;
    }
}

void Skeleton::recalcCurrentTransformPartial(uint fromNode)
{
    //not updating fromNode itself

    uint index = fromNode + 1u;

    auto it_node = (m_nodes.begin()+ index);

    for (; it_node != m_nodes.end(); ++it_node) {
        uint parent = (*it_node).parent;
        if (parent < fromNode) break;

        Matrix4x4 const& parent_transform = m_nodeTransforms[(*it_node).parent];
        m_nodeTransforms[index] = parent_transform * m_nodes[index].transform;

        ++index;
    }
}

void Skeleton::setParentTransform(Matrix4x4 const& matrix)
{
    m_parentTransform = matrix;
}

void Skeleton::addMesh(Mesh* pMesh)
{
    m_meshes.push_back(pMesh);
}

void Skeleton::DebugDraw(GraphicsContext& context, RenderShader* shader, Mesh* mesh)
{
    //Matrix4x4 scaleDebug = Matrix4x4::CreateScale(0.05f, 0.05f, 0.05f);
    Matrix4x4 scaleDebug = Matrix4x4::CreateScale(1.0f, 1.0f, 1.0f);

    for (auto it = m_bones.begin(); it != m_bones.end(); ++it) {
        uint nodeIndex = (*it).linkedNode;
        Matrix4x4 const& nodeTransform = m_nodeTransforms[nodeIndex];
        context.setCurrentShader(shader);
        shader->transferByName("uModelMatrix", scaleDebug * nodeTransform);
        mesh->Draw();    
    }

    for (uint nodeIndex{ 0u }; nodeIndex < m_nodes.size(); ++nodeIndex) {
        uint parentIndex = m_nodes[nodeIndex].parent;
        if (parentIndex != 0u) {
            Matrix4x4 const& nodeTransform = m_nodeTransforms[nodeIndex];
            Vector3 nodePos = Vector3(nodeTransform(0, 3), nodeTransform(1, 3), nodeTransform(2, 3));
            Matrix4x4 const& parentTransform = m_nodeTransforms[parentIndex];
            Vector3 parentPos = Vector3(parentTransform(0, 3), parentTransform(1, 3), parentTransform(2, 3));
            context.drawLine(nodePos, parentPos, Color(1.0f, 0.0f, 0.0f, 1.0f));
        }
    }
}

void Skeleton::Draw(RenderShader* shader)
{

    shader->transferArray("uBoneMatrix", m_boneTransforms);

    for (auto it = m_meshes.begin(); it != m_meshes.end(); ++it) {
        (*it)->Draw();
    }
}

void Skeleton::setTransform(Matrix4x4 const& transform)
{
    m_transform = transform;
}

void Skeleton::bindPose()
{
    for (uint i = 0; i < m_nodes.size(); ++i) {
        m_nodes[i].transform = m_bindPose[i];
    }
}

Vector3 Skeleton::nodePosition(uint index) const
{
    return Vector3(m_nodeTransforms[index](0, 3), m_nodeTransforms[index](1, 3), m_nodeTransforms[index](2, 3));
}

Vector3 Skeleton::bindPoseNodeOrientation(uint index) const
{
    return Vector3(m_bindPose[index](0, 1), m_bindPose[index](1, 1), m_bindPose[index](2, 1));
}

Vector3 Skeleton::currentNodeUp(uint index) const
{
    return Vector3(m_nodeTransforms[index](0, 1), m_nodeTransforms[index](1, 1), m_nodeTransforms[index](2, 1));
}




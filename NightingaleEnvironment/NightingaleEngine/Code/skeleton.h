#pragma once
#include "defines.h"
#include "ngmath.h"

//TO DO remove these forward declarations
class RenderShader;
class Mesh;
class Animation;
class GraphicsContext;

class Skeleton {
    friend class SkeletonBuilder;
    friend class AnimationEvaluator;
    friend class InverseKinematicEvaluator;
private:
    struct SkelBone;
    struct SkelNode;
public:

    void calcCurrentTransform();
    void calcBoneMatricies();

    void recalcCurrentTransformPartial(uint fromNode);

    void setParentTransform(Matrix4x4 const&);
    void addMesh(Mesh*);

    void DebugDraw(GraphicsContext& context, RenderShader*, Mesh*);
    void Draw(RenderShader*);


    void setTransform(Matrix4x4 const&);

    void bindPose();

    Vector3 const& nodePosition(uint index) const;
    Vector3 const& bindPoseNodeOrientation(uint index) const;
    Vector3 const& currentNodeUp(uint index) const;

private:


    std::vector<SkelBone> m_bones{};
    std::vector<SkelNode> m_nodes{};

    std::vector<Matrix4x4> m_bindPose{};

    //current node and bone transforms
    std::vector<Matrix4x4> m_nodeTransforms{};
    std::vector<Matrix4x4> m_boneTransforms{};

    std::vector<Mesh*> m_meshes{};


    //internal transform SET ONCE IN THE IMPORTER
    Matrix4x4 m_parentTransform{};

    //transform of the world coordinate skeleton
    Matrix4x4 m_transform{};

    std::map<string, uint> m_nodeNameMap{}; //TO DO: TEMPORARY, REMOVE LATER

    struct SkelBone {
        uint linkedNode;
        Matrix4x4 offsetMatrix;
    };

    struct SkelNode {
        uint parent;
        Matrix4x4 transform; //in local space!

    };
};

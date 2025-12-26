#pragma once
#include "skeleton.h"

class GraphicsContext;

class InverseKinematicEvaluator {
    
public:

    struct Limb {
        Vector3 from;
        Vector3 to;

        uint fromIdx;
        uint toIdx;

        float length;
    };

    Skeleton* m_pSkeleton{nullptr};

    bool m_bEnabled{ false };
    bool m_bDrawEndEffector{ false };
    bool m_bDrawIKBones{ false };

    bool m_bIKOutsideOfDistance{ true };

    vector<Limb> m_limbs;

    Vector3 m_IKTarget{};
    Vector3 m_absoluteTarget{};

    Vector3 m_reverseTarget{};
    float m_totalLength{ 0.0f };

    uint m_maxFabricIteration{ 4 };
    float m_endEffectorDeltaTolerance{ 0.01f };

    Vector3 const& endEffector() const;
    Vector3 const& root() const;

    void bind(Skeleton* pSkeleton, string const& bindNode, uint bindDepth);
    void evaluate();
    void draw(GraphicsContext&);

    void prepare();
    void fabricForward();
    void fabricBackward();

    void setSkeletonOrientations();
};
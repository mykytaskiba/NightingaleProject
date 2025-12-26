#pragma once
#include "render_node.h"
#include "defines.h"
#include "vector.h"
#include "cubic_spline.h"
#include "arc_length_table.h"

class ProjectOneGameObject;
class ProjectTwo;
class Mesh;
class RenderShader;

class ProjectTwoRenderHook : public RenderNode {
public:
    ProjectTwo& m_projectTwo;
    ProjectTwoRenderHook(ProjectTwo& prjTwo) : m_projectTwo(prjTwo) {}

    SplineRepresentation m_curveRepresentation;

    virtual void render(GraphicsContext& context);

};

class ProjectTwo {
    ProjectOneGameObject& m_projectOne;
public:
    ProjectTwo(ProjectOneGameObject&);
    void init();
    void tick();
    void shutdown();

    void originalPath();

    void setPathToPoint(Vector3 const& point);

    float easeineaseoutFunc(float t) const;
    float easeineaseoutSpeedFunc(float t) const;

    Mesh* m_pNodeModel{ nullptr };
    RenderShader* m_pShader{ nullptr };

    vector<Vector3> m_curvePoints{};
    Vector3 m_evaluatedPoint;

    CubicSpline3D m_curve;
    ArcLengthTable m_arcLengthTable;

    bool m_bPlaybackActive{ false };
    float m_tDistValue{ 0.0f };
    float m_maxAnimationSpeed{ 0.0f };

    float m_animationSpeedFactor{ 1.6f };
    
    float m_curveLengthSpeedFactor{0.0f};
    float m_travelSpeed{ 0.05f };
    
    bool m_bUseEaseInEaseOut{ true };

    bool m_uiActive{ false };

    bool m_drawCurve{ false };

    void activate();

    ProjectTwoRenderHook m_renderHook; //TO DO: eventually remove this
};

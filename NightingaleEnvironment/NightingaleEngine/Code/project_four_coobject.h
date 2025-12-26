#pragma once
#include "render_node.h"
#include "defines.h"
#include "vector.h"
#include "transform.h"

class ProjectOneGameObject;
class ProjectFour;
class Mesh;
class RenderShader;
class RenderSkinnedMeshNode;

class ProjectFourRenderHook : public RenderNode {
public:
    ProjectFour& m_projectFour;
    ProjectFourRenderHook(ProjectFour& prj) : m_projectFour(prj) {}

    virtual void render(GraphicsContext& context);

    void initRaw();
};

class ProjectFour {
    ProjectOneGameObject& m_projectOne;

    struct Tetrahedron;
    struct SpringCons {
        float restLength{};
        float stiffness{ 1.0f };

        uint v0{ 0u };
        uint v1{ 0u };

        void createConstraint(ProjectFour& state, uint inv0, uint inv1);

        void simulateConstraint(ProjectFour& state);
    };

    struct TetraCons {
        float restVolume{};
        float stiffness{ 1.0f };

        void createConstraint(ProjectFour& state, Tetrahedron&);
        float getVolume(ProjectFour& state, Tetrahedron&);

        void simulateConstraint(ProjectFour& state, Tetrahedron&);
    };

    struct Tetrahedron {


        Tetrahedron(uint _v0, uint _v1, uint _v2, uint _v3);
        uint v0;
        uint v1;
        uint v2;
        uint v3;

        ProjectFour::TetraCons volumeCons;

        void createConstraints(ProjectFour&);

        void draw(GraphicsContext&, ProjectFour&);

        void simulateConstraints(ProjectFour&);


    };


    struct SimulatedMass {
        Vector3 position{ 0.0f,0.0f,0.0f };
        Vector3 velocity{ 0.0f,0.0f,0.0f };

        Vector3 force{ 0.0f,0.0f,0.0f };

        bool inContact{ false };

        Vector3 prevPosition{ 0.0f,0.0f,0.0f };

        float mass{ 1.0f };

        void simulate(float deltaT, ProjectFour& state);
    };

public:
    ProjectFour(ProjectOneGameObject&);
    void init();
    void tick();
    void shutdown();

    void kickUp();
    void kickForward();

    void render(GraphicsContext& context);
    void physicsTick(float deltaT);

    void carry(float deltaT);

    bool m_uiActive{ true };

    vector<SimulatedMass> m_simulatedMasses;
    vector<Tetrahedron> m_tetrahedrons;
    Transform m_objectTransform;

    uint addMassPoint(Vector3 const&);
    void addSpring(uint from, uint to);

    SimulatedMass& getMass(uint index);
    Vector3 worldPoint(uint index) const;
    Vector3 worldVec(uint from, uint to) const;

    float m_floorHeight = -12.5f;

    Vector3 m_gravity{ 0.0f,-9.8f,0.0f };
    float m_gravityScale{ 1.0f };

    Vector3 m_centerOfMass{ 0.0f,0.0f,0.0f };
    bool m_bDrawCOM{ false };
    int m_numCollisionThisSimulation{ 0 };
    int m_numCollisionLastSimulation{ 0 };
    float m_massSum{ 0.0f };

    vector<SpringCons> m_springs{};

    int m_numMasses{ 0 };
    int m_numSprings{ 0 };
    int m_numVolumes{ 0 };

    Vector3 m_startPosition{ 5.0f,5.0f,5.0f };
    float m_startScale{ 1.5f };
    float m_startAngle{ 0.0f };
    int m_modelComplexity{ 4 };
    void resetObject();
    void addCube( float size, Vector3 offset );

    bool m_bSimulate{ false };

    bool m_bCarryBox{ false };
    Vector3 carryPosition();
    Vector3 carryPositionLeftHand();
    Vector3 carryPositionRightHand();

    int m_numPhysicsSteps{ 3 };

    float m_currentDeltaT{ 0.0f };

    float m_springStiffness{ 500.0f };
    float m_volumeStiffness{ 750.0f };
    float m_elasticity{ 0.0f };
    float m_surfaceFriction{ 0.25f };

    float m_boxCarrySpeed{ 16.0f };
    float m_boxPickupDist{ 10.0f };

    float m_kickForce{ 50.0f };
    float m_kickRange{ 3.0f };

    float m_compress{ 0.0f };

    void Kick(float forwardFactor, float upFactor);
    Vector3 getKickForce(float forwardFactor, float upFactor);

    void OnDisableCarry();
    void OnCarryButton();

    void ThrowBox();


    ProjectFourRenderHook m_renderHook; //TO DO: eventually remove this
};

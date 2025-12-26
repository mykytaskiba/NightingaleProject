#include "pch.h"
#include "project_four_coobject.h"
#include "engine_internals.h"
#include "time.h"
#include "project_one_gameobject.h"

ProjectFour::ProjectFour(ProjectOneGameObject& projectOne) : m_projectOne(projectOne), m_renderHook(*this)
{
}

void ProjectFour::init()
{
    EngineInternals::Renderer().registerRenderable(&m_renderHook); // hack for rendering with a specific render hook
    m_renderHook.initRaw();

    resetObject();


}

void ProjectFour::tick()
{
    ImGui::NewLine();
    ImGui::Checkbox("Project Four", &m_uiActive);
    if (m_uiActive) {

        ImGui::Checkbox("Simulate Physics", &m_bSimulate);
        ImGui::SliderInt("Number Of Physics Substeps", &m_numPhysicsSteps, 1, 5);
        ImGui::Checkbox("DrawCOM", &m_bDrawCOM);
        ImGui::Text("Number Active Masses: %i", m_numMasses);
        ImGui::Text("Number Active Springs: %i", m_numSprings);
        ImGui::Text("Number Active Volumes: %i", m_numVolumes);

        ImGui::NewLine();
        ImGui::SliderFloat3("Start Position", &m_startPosition[0], - 20.0f, 20.0f);
        ImGui::SliderFloat("Start Scale", &m_startScale, 0.1f, 3.0f);
        ImGui::SliderFloat("Start Angle", &m_startAngle, 0.0f, 360.0f);
        ImGui::SliderInt("Model Complexity", &m_modelComplexity, 1, 6);

        ImGui::NewLine();
        ImGui::SliderFloat("Gravity Scale", &m_gravityScale, 0.0f, 5.0f);
        ImGui::SliderFloat("Spring Stiffness", &m_springStiffness, 5.0f, 4000.0f);
        ImGui::SliderFloat("Volume Stiffness", &m_volumeStiffness, 5.0f, 4000.0f);
        //ImGui::SliderFloat("Elasticity", &m_elasticity, 0.0f, 1.5f);
        ImGui::SliderFloat("Surface Friction", &m_surfaceFriction, 0.0f, 3.0f);
        ImGui::SliderFloat("Compress Factor", &m_compress, -1.0f, 1.0f);

        
        
        ImGui::NewLine();
        ImGui::SliderFloat("Kick Force", &m_kickForce, 1.0f, 200.0f);
        ImGui::SliderFloat("Kick Range", &m_kickRange, 1.0f, 5.0f);
        if (ImGui::Button("Reset Object")) {
            resetObject();
        }

        if (ImGui::Button("Simulate One Step (60fps)")) {
            physicsTick(1.0f/60.0f);
        }
    }


    if (m_bSimulate) {
        physicsTick(Time::GetDelta());
    }
}

void ProjectFour::shutdown()
{
}

void ProjectFour::kickUp()
{
    Kick(0.1f, 0.9f);
}

void ProjectFour::kickForward()
{
    Kick(0.9f, 0.1f);
}

void ProjectFour::render(GraphicsContext& context)
{

    for (uint i{ 0u }; i < m_simulatedMasses.size(); ++i) {
        context.drawSphere(worldPoint(i), 0.05f);
    }
    for (uint i{ 0u }; i < m_tetrahedrons.size(); ++i) {
        m_tetrahedrons[i].draw(context, *this);
    }

    if (m_bDrawCOM) {
        context.drawSphere(m_centerOfMass, 0.5f, Color(0.2f, 0.2f, 0.6f, 1.0f));
    }

    context.drawSphere(m_startPosition, m_startScale * 0.5f, Color(0.8f, 0.2f, 0.2f, 1.0f));


}

void ProjectFour::physicsTick(float deltaT)
{

    float totalDeltaT = deltaT;
    deltaT = totalDeltaT / ((float)m_numPhysicsSteps); //delta t is now physics substep deltaT
    m_currentDeltaT = deltaT;

    

    for (int step{ 0 }; step < m_numPhysicsSteps; ++step) {
        
        carry(deltaT);

        m_numCollisionThisSimulation = 0;
        
        m_centerOfMass *= 0.0f;
        for (SimulatedMass& mass : m_simulatedMasses) {
            mass.simulate(deltaT, *this);
            
            m_centerOfMass += mass.position / mass.mass;
        }


        m_centerOfMass /= ((float) m_simulatedMasses.size());

        m_numCollisionLastSimulation = m_numCollisionThisSimulation;

        for (Tetrahedron& tetra : m_tetrahedrons) {
            tetra.simulateConstraints(*this);
        }
        for (SpringCons& spring : m_springs) {
            spring.simulateConstraint(*this);
        }

        for (SimulatedMass& mass : m_simulatedMasses) {
            mass.velocity = (mass.position - mass.prevPosition)/deltaT;
        }


    }
    //m_objectTransform.rotation *= Quaternion(Vector3(1.0f, 1.0f, 1.0f).normalized(), 0.1f * deltaT);
}

void ProjectFour::carry(float deltaT)
{
    if (!m_bCarryBox) return;

    Vector3 carryPos = carryPosition();

    for (SimulatedMass& mass : m_simulatedMasses) {
        Vector3 toCarryPos = (carryPos - m_centerOfMass).normalized() * m_boxCarrySpeed;
        Vector3 velocity = toCarryPos + m_gravity * -m_gravityScale * deltaT;
        mass.velocity = velocity;
    }

    m_projectOne.m_ProjectThree.OverridePosition(carryPositionLeftHand(), carryPositionRightHand());
}

uint ProjectFour::addMassPoint(Vector3 const& vector)
{
    SimulatedMass mass;
    mass.position = m_objectTransform.transform(vector);
    mass.prevPosition = mass.position;
    m_massSum += mass.mass;

    uint index = m_simulatedMasses.size();

    for (uint i{ 0u }; i < m_simulatedMasses.size(); ++i) {
        if ((m_simulatedMasses[i].position - mass.position).magnitude() < 0.01f) {
            index = i;
            break;
        }
    }
    if (index == m_simulatedMasses.size()) {
        m_simulatedMasses.push_back(mass);
    }
    return index;


}

void ProjectFour::addSpring(uint from, uint to)
{
    for (SpringCons& spring : m_springs) {
        if (spring.v0 == from && spring.v1 == to) {
            return;
        }
        if (spring.v0 == to && spring.v1 == from) {
            return;
        }
    }

    SpringCons spring;
    spring.createConstraint(*this, from, to);
    m_springs.push_back(spring);
}

ProjectFour::SimulatedMass& ProjectFour::getMass(uint index)
{
    return m_simulatedMasses[index];
}

Vector3 ProjectFour::worldPoint(uint index) const
{
    return m_simulatedMasses[index].position;
}

Vector3 ProjectFour::worldVec(uint from, uint to) const
{
    return worldPoint(to)-worldPoint(from);
}

void ProjectFour::resetObject()
{

    m_tetrahedrons.clear();
    m_simulatedMasses.clear();
    m_springs.clear();

    m_centerOfMass = m_startPosition;
    m_massSum = 0.0f;

    m_objectTransform.position = m_startPosition;
    m_objectTransform.scale = m_startScale;
    m_objectTransform.rotation = Quaternion(Vector3(1.0f, 1.0f, 1.0f).normalized(), m_startAngle / 360.0f * 2.0f * pi);


    //addCube(1.0f, Vector3(0.0f, 0.0f, 0.0f));
    assert(m_modelComplexity > 0);
    float scaleFactor = 1.0f / (float)m_modelComplexity;
    float modelDelta = 2.0f * scaleFactor;

    Vector3 globalOffset =  (modelDelta / 2.0f - 1.0f) * Vector3(1.0f, 1.0f, 1.0f);

    for (uint i{ 0u }; i < m_modelComplexity; ++i) {
        for (uint j{ 0u }; j < m_modelComplexity; ++j) {
            for (uint k{ 0u }; k < m_modelComplexity; ++k) {

                addCube(scaleFactor, modelDelta * Vector3(i, j, k) + globalOffset);

            }
        }
    }


    for (Tetrahedron& tetra : m_tetrahedrons) {
        tetra.createConstraints(*this);
    }
    m_numMasses = m_simulatedMasses.size();
    m_numSprings = m_springs.size();
    m_numVolumes = m_tetrahedrons.size();
}

void ProjectFour::addCube(float size, Vector3 offset)
{
    Transform cubeSpace;
    cubeSpace.scale = size;
    cubeSpace.position = offset;

    uint i0 = addMassPoint(cubeSpace.transform(Vector3(-1.0f, -1.0f, -1.0f))); //0 shared with 2 lower
    uint i1 = addMassPoint(cubeSpace.transform(Vector3(-1.0f, +1.0f, -1.0f))); //1 first only
    uint i2 = addMassPoint(cubeSpace.transform(Vector3(+1.0f, +1.0f, -1.0f))); //2 shared with 1 lower
    uint i3 = addMassPoint(cubeSpace.transform(Vector3(-1.0f, +1.0f, +1.0f))); //3 shared with 1 lower

    uint i4 = addMassPoint(cubeSpace.transform(Vector3(+1.0f, +1.0f, +1.0f))); //4 second only
    uint i5 = addMassPoint(cubeSpace.transform(Vector3(+1.0f, -1.0f, +1.0f))); //5 shared with lower

    uint i6 = addMassPoint(cubeSpace.transform(Vector3(+1.0f, -1.0f, -1.0f))); //6 only --> 2

    uint i7 = addMassPoint(cubeSpace.transform(Vector3(-1.0f, -1.0f, +1.0f))); //7 only --> 3

    Tetrahedron tetrahedron(0, 0, 0, 0);

    tetrahedron = Tetrahedron(i0, i1, i2, i3);
    m_tetrahedrons.push_back(tetrahedron);

    tetrahedron = Tetrahedron(i2, i3, i4, i5);
    m_tetrahedrons.push_back(tetrahedron);

    tetrahedron = Tetrahedron(i0, i5, i6, i2);
    m_tetrahedrons.push_back(tetrahedron);

    tetrahedron = Tetrahedron(i0, i5, i7, i3);
    m_tetrahedrons.push_back(tetrahedron);
}

Vector3 ProjectFour::carryPosition()
{
    return m_projectOne.getTransform().transform(Vector3(0, 12.5f, 3.5f));
}

Vector3 ProjectFour::carryPositionLeftHand()
{
    Vector3 diff = m_centerOfMass - carryPosition();
    return m_projectOne.getTransform().transform(Vector3(1.0f, 11.0f, 3.0f)) + diff;
    
}

Vector3 ProjectFour::carryPositionRightHand()
{
    Vector3 diff = m_centerOfMass - carryPosition();
    return m_projectOne.getTransform().transform(Vector3(-1.0f, 11.0f, 3.0f)) + diff;
}

void ProjectFour::Kick(float forwardFactor, float upFactor)
{
    if (!m_bSimulate) return;
    Vector3 toVec = m_centerOfMass - m_projectOne.getTransform().position;
    toVec[1] = 0.0f;
    if (toVec.magnitude() > m_kickRange) return;

    Vector3 force = getKickForce(forwardFactor, upFactor);

    for (SimulatedMass& mass : m_simulatedMasses) {
        mass.velocity += force / mass.mass;
    }
}

Vector3 ProjectFour::getKickForce(float forwardFactor, float upFactor)
{
    Vector3 position = m_projectOne.getTransform().position;
    Vector3 toVec = (m_centerOfMass - position);
    toVec[1] = 0.0f;
    toVec.normalize_equal();
    return m_kickForce * (toVec * forwardFactor + upFactor * Vector3(0.0f, 1.0f, 0.0f));

}

void ProjectFour::OnDisableCarry()
{
    for (SimulatedMass& mass : m_simulatedMasses) {
        mass.velocity = Vector3(0.0f, 0.0f, 0.0f);
    }
    m_projectOne.m_ProjectThree.DisableOverride();
}

void ProjectFour::OnCarryButton()
{

    if (!m_bSimulate) return;

    if (!m_bCarryBox) {
        Vector3 distVec = carryPosition() - m_centerOfMass;
        if (distVec.magnitude() < m_boxPickupDist) {
            m_bCarryBox = true;
        }
    }
    else {
        m_bCarryBox = false;
        OnDisableCarry();

        Vector3 lightForce = getKickForce(0.1f,0.1f);

        for (SimulatedMass& mass : m_simulatedMasses) {
            mass.velocity += lightForce / mass.mass;
        }
    }


}

void ProjectFour::ThrowBox()
{
    if (!m_bSimulate) return;
    if (!m_bCarryBox) return;

    m_bCarryBox = false;
    OnDisableCarry();

    Vector3 force = getKickForce(sqrt(2.0f)/2.0f, sqrt(2.0f)/2.0f);

    for (SimulatedMass& mass : m_simulatedMasses) {
        mass.velocity += force / mass.mass;
    }

    m_projectOne.setAnimation(m_projectOne.m_pAnimThrow);
    m_projectOne.m_bPlaying = true;
}

void ProjectFourRenderHook::render(GraphicsContext& context)
{
    m_projectFour.render(context);
}

void ProjectFourRenderHook::initRaw()
{
}

ProjectFour::Tetrahedron::Tetrahedron(uint _v0, uint _v1, uint _v2, uint _v3) 
    : v0(_v0), v1(_v1), v2(_v2), v3(_v3)
{
}


void ProjectFour::Tetrahedron::createConstraints(ProjectFour& state)
{
    state.addSpring(v0, v1);
    state.addSpring(v0, v2);
    state.addSpring(v0, v3);
    state.addSpring(v1, v2);
    state.addSpring(v1, v3);
    state.addSpring(v2, v3);
    volumeCons.createConstraint(state, *this);
}

void ProjectFour::Tetrahedron::draw(GraphicsContext& context, ProjectFour& state)
{
    context.drawLine(state.worldPoint(v0), state.worldPoint(v1));
    context.drawLine(state.worldPoint(v0), state.worldPoint(v2));
    context.drawLine(state.worldPoint(v0), state.worldPoint(v3));

    context.drawLine(state.worldPoint(v1), state.worldPoint(v2));
    context.drawLine(state.worldPoint(v1), state.worldPoint(v3));
    context.drawLine(state.worldPoint(v2), state.worldPoint(v3));
    
    
}

void ProjectFour::Tetrahedron::simulateConstraints(ProjectFour& state)
{
    volumeCons.simulateConstraint(state, *this);
}

void ProjectFour::SimulatedMass::simulate(float deltaT, ProjectFour& state)
{


    velocity += state.m_gravity * state.m_gravityScale * deltaT;
    if (inContact) {
        velocity[1] = 0.0f;
        inContact = false;
        Vector3 flatVelocity = velocity;
        flatVelocity[1] = 0.0f;
        float flatVelocityMagn = flatVelocity.magnitude();
        if (flatVelocityMagn > 1.0f) {
            flatVelocity.normalize_equal();
        }

        assert(state.m_numCollisionLastSimulation > 0);
        float collisionNumFactor = state.m_massSum / ((float)state.m_numCollisionLastSimulation);

        float frictionN = -1.0f*state.m_gravity[1]*state.m_gravityScale * collisionNumFactor;
        Vector3 forceFriction = state.m_surfaceFriction * -1.0f * flatVelocity * frictionN;
        Vector3 appliedFriction = forceFriction * deltaT;
        if (appliedFriction.magnitude() > flatVelocityMagn) {
            appliedFriction.normalize_equal();
            appliedFriction *= flatVelocityMagn;
        }
        velocity += appliedFriction;

        
        //velocity += collisionNumFactor/mass * force * state.m_elasticity;
        //force = Vector3(0.0f, 0.0f, 0.0f);
        

    }

    prevPosition = position;
    Vector3 nextPosition = position + velocity * deltaT;

    if (nextPosition[1] < state.m_floorHeight) {
        // 1/2 m * v^2 * elasticity factor
        inContact = true;
        ++state.m_numCollisionThisSimulation;

        //force = Vector3(0, -velocity[1], 0.0f);
        nextPosition[1] = state.m_floorHeight;
        //return;
    }
    position = nextPosition;
}

void ProjectFour::SpringCons::createConstraint(ProjectFour& state, uint inv0, uint inv1)
{
    v0 = inv0;
    v1 = inv1;
    restLength = (state.worldPoint(v1) - state.worldPoint(v0)).magnitude();
}

void ProjectFour::SpringCons::simulateConstraint(ProjectFour& state)
{
    float deltaT = state.m_currentDeltaT;
    float dist = (state.worldPoint(v1) - state.worldPoint(v0)).magnitude();
    float C = restLength - dist;
    Vector3 C0 = (state.worldPoint(v1) - state.worldPoint(v0)) / dist;
    Vector3 C1 = -1.0 * C0;

    float w0 = 1.0f/state.getMass(v0).mass;
    float w1 = 1.0f/state.getMass(v1).mass;

    float alpha = 1.0f / state.m_springStiffness; //inverse of stiffness?
    float alphaTerm = alpha / (deltaT * deltaT);

    float lambda = -C / (w0 * C0.magnitude_squared() + w1 * C1.magnitude_squared() + alphaTerm);

    Vector3 deltaX0 = lambda * w0 * C0;
    Vector3 deltaX1 = lambda * w1 * C1;
    
    state.getMass(v0).position += deltaX0;
    state.getMass(v1).position += deltaX1;

    
}

void ProjectFour::TetraCons::createConstraint(ProjectFour& state, Tetrahedron& tetra)
{
    restVolume = getVolume(state, tetra);
}

float ProjectFour::TetraCons::getVolume(ProjectFour& state, Tetrahedron& tetra)
{
    Vector3 vec1 = state.worldPoint(tetra.v1) - state.worldPoint(tetra.v0);
    Vector3 vec2 = state.worldPoint(tetra.v2) - state.worldPoint(tetra.v0);
    Vector3 vec3 = state.worldPoint(tetra.v3) - state.worldPoint(tetra.v0);

    Vector3 crossVec = vec1.cross(vec2);

    return (1.0f / 6.0f * crossVec.dot(vec3));
}

void ProjectFour::TetraCons::simulateConstraint(ProjectFour& state, Tetrahedron& tetra)
{
    float deltaT = state.m_currentDeltaT;
    float currVolume = getVolume(state, tetra) * (1.0f + state.m_compress);

    float C = 6.0f * (currVolume - restVolume);

    Vector3 C0 = state.worldVec(tetra.v3, tetra.v1).cross(state.worldVec(tetra.v2, tetra.v1));
    Vector3 C1 = state.worldVec(tetra.v2, tetra.v0).cross(state.worldVec(tetra.v3, tetra.v0));
    Vector3 C2 = state.worldVec(tetra.v3, tetra.v0).cross(state.worldVec(tetra.v1, tetra.v0));
    Vector3 C3 = state.worldVec(tetra.v1, tetra.v0).cross(state.worldVec(tetra.v2, tetra.v0));

    float w0 = 1.0f / state.getMass(tetra.v0).mass;
    float w1 = 1.0f / state.getMass(tetra.v1).mass;
    float w2 = 1.0f / state.getMass(tetra.v2).mass;
    float w3 = 1.0f / state.getMass(tetra.v3).mass;

    float alpha = 1.0f / state.m_volumeStiffness;//inverse of stiffness?
    float alphaTerm = alpha / (deltaT * deltaT);

    float lambda = 
        -C 
        / (
            w0 * C0.magnitude_squared() +
            w1 * C1.magnitude_squared() + 
            w2 * C2.magnitude_squared() +
            w3 * C3.magnitude_squared() +
            alphaTerm);

    Vector3 deltaX0 = lambda * w0 * C0;
    Vector3 deltaX1 = lambda * w1 * C1;
    Vector3 deltaX2 = lambda * w1 * C2;
    Vector3 deltaX3 = lambda * w1 * C3;

    state.getMass(tetra.v0).position += deltaX0;
    state.getMass(tetra.v1).position += deltaX1;
    state.getMass(tetra.v2).position += deltaX2;
    state.getMass(tetra.v3).position += deltaX3;
}
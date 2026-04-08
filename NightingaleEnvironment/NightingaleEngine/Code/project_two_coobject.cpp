#include "pch.h"
/*
#include "project_two_coobject.h"
#include "engine_internals.h"
#include "renderer.h"
#include "asset_manager.h"
#include "mesh.h"
#include "render_shader.h"
#include "defines.h"
#include "project_one_gameobject.h"
#include "engine_functions.h"
#include "ngmath.h"

ProjectTwo::ProjectTwo(ProjectOneGameObject& projectOne) : m_projectOne(projectOne), m_renderHook(*this)
{
}

void ProjectTwo::init()
{
    EngineInternals::Renderer().registerRenderable(&m_renderHook); // hack for rendering with a specific render hook

    m_pNodeModel = AssetManager<Mesh>::retrieve("sphere_mesh");
    assert(m_pNodeModel != nullptr);

    m_pShader = AssetManager<RenderShader>::retrieve("unlit_shader");
    assert(m_pShader != nullptr);

    originalPath();


}

void ProjectTwo::tick()
{
    //ImGui::BeginChild("ProjectTwoPanel");
    ImGui::NewLine();
    ImGui::Checkbox("Project Two", &m_uiActive);

    if (m_uiActive) {
        if (ImGui::Checkbox("Playback", &m_bPlaybackActive)) {

            if (m_bPlaybackActive) {
                activate();
            }

        }
        ImGui::Checkbox("Draw Curve", &m_drawCurve);
        if (ImGui::Checkbox("Ease In East Out", &m_bUseEaseInEaseOut)) {
            m_bPlaybackActive = false;
        }
        if (ImGui::SliderFloat("T Distance Value", &m_tDistValue, 0.0f, 1.0f)) {
            m_bPlaybackActive = false;
            float distValue = m_tDistValue * m_arcLengthTable.getTotalLength();
            m_evaluatedPoint = m_curve.evaluate(m_arcLengthTable.getTFromLength(distValue));
        }
        ImGui::SliderFloat("Animation Speed Factor", &m_animationSpeedFactor, 0.0f, 10.0f);
        if (ImGui::Button("Reset Original Path")) {
            originalPath();
        }
    }
    //ImGui::EndChild();

    if (m_bPlaybackActive) {

        m_curveLengthSpeedFactor = m_arcLengthTable.getTotalLength() / 200.0f; //magic number in this case

        m_tDistValue += m_travelSpeed * 1.0f / m_curveLengthSpeedFactor * EngineFunctions::delta_time();
        if (m_tDistValue >= 1.0f) {
            m_tDistValue = 1.0f;
            m_bPlaybackActive = false;
        }

        float tPostEaseIn = m_tDistValue;
        float animationSpeedFactor = 1.0f;
        if (m_bUseEaseInEaseOut) {
            tPostEaseIn = easeineaseoutFunc(m_tDistValue);
            animationSpeedFactor = easeineaseoutSpeedFunc(m_tDistValue);
        }
        animationSpeedFactor *= m_animationSpeedFactor;
        //animationSpeedFactor *= m_curveLengthSpeedFactor;

        m_projectOne.m_playbackSpeed = m_maxAnimationSpeed*animationSpeedFactor;

        float distValue = tPostEaseIn * m_arcLengthTable.getTotalLength();
        float evaluatedT = m_arcLengthTable.getTFromLength(distValue);
        m_evaluatedPoint = m_curve.evaluate(evaluatedT);

        Vector3 current = m_evaluatedPoint;
        m_projectOne.getTransform().position = current + Vector3(0.0f, -5.5f, 0.0f);


        Vector3 pointVec;
        if (evaluatedT < 0.5f) {
            pointVec = m_curve.evaluate(evaluatedT + 0.02f) - current;
        }
        else {
            pointVec = current - m_curve.evaluate(evaluatedT - 0.02f);
        }
        pointVec.normalize_equal();
        m_projectOne.getTransform().rotation = Quaternion(Vector3(0.0f, 0.0f, 1.0f), pointVec);


    }
}

void ProjectTwo::shutdown()
{
}

void ProjectTwo::originalPath()
{
    m_arcLengthTable = ArcLengthTable();
    m_curve = CubicSpline3D();
    m_curvePoints.clear();

    for (uint i{ 0u }; i < 8u; ++i) {

        float dist = 24.0f;
        if (i % 2u == 0u) {
            dist += 3.0f;
        }
        if (i % 3u == 0u) {
            dist += 4.0f;
        }
        if (i % 5u == 0u) {
            dist += 12.0f;
        }
        if (i % 6u == 0u) {
            dist += 13.0f;
        }
        float angle = pi * 2.0f * ((float)i) / 8.0f;
        Vector3 vec(dist * cos(angle), -12.0f, dist * sin(angle));
        m_curvePoints.push_back(vec);
        m_curve.addValue(vec);
    }

    m_curve.finalize();

    m_arcLengthTable.evaluate(m_curve);

    m_evaluatedPoint = m_curvePoints[0];

    m_renderHook.m_curveRepresentation.represent(m_curve, 0.01f);
}

void ProjectTwo::setPathToPoint(Vector3 const& point)
{
    m_arcLengthTable = ArcLengthTable();
    m_curve = CubicSpline3D();
    m_curvePoints.clear();

    m_tDistValue = 0.0f;

    Vector3 at = m_projectOne.getTransform().position;
    at[1] = -12.0f;

    Vector3 finish = point;
    finish[1] = -12.0f;

    Vector3 half = (at + finish) / 2.0f;
    float halfDist = (finish - at).magnitude()/2.0f;
    Vector3 toFinish = (finish - at).normalized();

    Matrix4x4 rotationMatrix = m_projectOne.getTransform().rotation.getRotation();
    Vector4 forward4 = rotationMatrix * Vector4(0.0f, 0.0f, 1.0f, 1.0f);
    Vector3 forward = Vector3(forward4[0], forward4[1], forward4[2]).normalized();

    Vector3 second = at + forward * halfDist;
    second[1] = -12.0f;
    

    m_curvePoints.push_back(at);
    m_curve.addValue(at);

    if (toFinish.dot(forward) < 0.5f) { //add a second point only if we are turned away from our destination
        m_curvePoints.push_back(second);
        m_curve.addValue(second);
    }

    m_curvePoints.push_back(half);
    m_curve.addValue(half);

    m_curvePoints.push_back(finish);
    m_curve.addValue(finish);

    m_curve.finalize();

    m_arcLengthTable.evaluate(m_curve);

    m_evaluatedPoint = m_curvePoints[0];

    m_renderHook.m_curveRepresentation.represent(m_curve, 0.01f);
}

float ProjectTwo::easeineaseoutSpeedFunc(float t) const
{
    float temp = 2.0f * t * t - 2.0f * t;
    temp = -temp / ((temp + 1.0f) * (temp + 1.0f));
    temp /= 2.0f;
    return temp;
}

void ProjectTwo::activate()
{
    m_tDistValue = 0.0f;

    m_bPlaybackActive = true;
    m_projectOne.setAnimation(m_projectOne.m_pAnimWalk);
    m_projectOne.m_bPlaying = true;
    m_maxAnimationSpeed = m_projectOne.m_playbackSpeed;
}

float ProjectTwo::easeineaseoutFunc(float t) const
{
    float t2 = t * t;
    return (t2) / (2.0f * (t2 - t) + 1.0f);
}

void ProjectTwoRenderHook::render(GraphicsContext& context)
{
    if (!m_projectTwo.m_drawCurve) return;
    Transform t;
    
    assert(false);
    //-->t.scale = 0.5f;
    t.position = Vector3(5.0f, -12.0f, 0.0f);
    //t.scale = 0.05f;
    
    Mesh* pNodeModel = m_projectTwo.m_pNodeModel;
    RenderShader* pShader = m_projectTwo.m_pShader;

    context.setCurrentShader(pShader);
    pShader->transferByName("uColor", Color(0.9f, 0.9f, 0.1f, 1.0f));


    for (auto it{ m_projectTwo.m_curvePoints.begin() }; it != m_projectTwo.m_curvePoints.end(); ++it) {
        t.position = *it;
        pShader->transferByName("uModelMatrix", t.getMatrix());
        pNodeModel->Draw();
    }


    pShader->transferByName("uColor", Color(0.9f, 0.1f, 0.9f, 1.0f));
    {
        t.position = m_projectTwo.m_evaluatedPoint;
        pShader->transferByName("uModelMatrix", t.getMatrix());
        pNodeModel->Draw();
    }

    //TO DO: reimplement this method outside of math library
    //m_curveRepresentation.draw(context);

    //render hook hack to make 
}

*/
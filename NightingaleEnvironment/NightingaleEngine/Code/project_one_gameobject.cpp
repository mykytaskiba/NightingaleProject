#include "pch.h"
/*
#include "project_one_gameobject.h"
#include "engine_functions.h"
#include "render_skinned.h"
#include "asset_manager.h"
#include "input.h"
#include "ngrender.h"
#include "camera_controller.h"


void ProjectOneGameObject::init()
{
    m_tValue = 0.0f;
    m_playbackSpeed = 0.1f;

    m_pAnimWalk = AssetManager<Animation>::retrieve("ybot_walk");
    assert(m_pAnimWalk != nullptr);

    m_pAnimIdle = AssetManager<Animation>::retrieve("ybot_idle");
    assert(m_pAnimIdle != nullptr);

    m_pAnimDance = AssetManager<Animation>::retrieve("ybot_dance");
    assert(m_pAnimDance != nullptr);

    m_pAnimKick = AssetManager<Animation>::retrieve("ybot_kick");
    assert(m_pAnimKick != nullptr);

    m_pAnimKickUp = AssetManager<Animation>::retrieve("ybot_kickup");
    assert(m_pAnimKickUp != nullptr);

    m_pAnimThrow = AssetManager<Animation>::retrieve("ybot_throw");
    assert(m_pAnimThrow != nullptr);

    m_currentAnimation = nullptr;

    m_ProjectTwo.init();
    m_ProjectThree.init();
    m_ProjectFour.init();

    resetPosition();
}

void ProjectOneGameObject::tick()
{

    Animation* nextAnimationState = m_currentAnimation;

    ImGui::Begin("ControlPanel");

    float FPS = 1.0f / EngineFunctions::delta_time();
    //FPS Estimation
    ImGui::Text("FrameRate: %.1f", FPS);

    //ImGui::BeginChild("ProjectOne");
    ImGui::NewLine();
    ImGui::Checkbox("Project One", &m_uiActive);
    if (m_uiActive) {
        ImGui::SliderFloat("t", &m_tValue, 0.0f, 1.0f);

        ImGui::Checkbox("Play Animation", &m_bPlaying);


        ImGui::SliderFloat("Speed", &m_playbackSpeed, 0.01f, 2.0f);

        ImGui::Checkbox("Draw Mesh", &m_bDrawMesh);
        ImGui::Checkbox("Draw Skeleton Nodes", &m_bDrawSkeleton);


        if (ImGui::Button("Animation: BindPose")) nextAnimationState = nullptr;
        if (ImGui::Button("Animation: Idle")) nextAnimationState = m_pAnimIdle;
        if (ImGui::Button("Animation: Walk")) nextAnimationState = m_pAnimWalk;
        if (ImGui::Button("Animation: Dance")) nextAnimationState = m_pAnimDance;
        if (ImGui::Button("Animation: KickUp")) nextAnimationState = m_pAnimKickUp;
        if (ImGui::Button("Animation: Kick")) nextAnimationState = m_pAnimKick;


        if (ImGui::Button("Reset Position")) resetPosition();
    }

    //ImGui::EndChild();
    


    RenderSkinnedMeshNode* pRenderNode = dynamic_cast<RenderSkinnedMeshNode*>(m_pRenderNode);
    if (pRenderNode == nullptr) {
        assert(0);
        return;
    }

    if (nextAnimationState != m_currentAnimation) {
        setAnimation(nextAnimationState);
    }

    if(m_bPlaying) {
        m_tValue += EngineFunctions::delta_time() * m_playbackSpeed;
        while (m_tValue > 1.0f) {
            m_tValue += -1.0f;
            if (m_currentAnimation == m_pAnimKick) {
                m_ProjectFour.kickForward();
                setAnimation(m_pAnimIdle); //simple state machine
                m_bPlaying = true;
            }
            if (m_currentAnimation == m_pAnimKickUp) {
                m_ProjectFour.kickUp();
                setAnimation(m_pAnimIdle); //simple state machine
                m_bPlaying = true;
            }
            if (m_currentAnimation == m_pAnimThrow) {
                setAnimation(m_pAnimIdle); //simple state machine
                m_bPlaying = true;
            }
        }
    }

    pRenderNode->setDrawMode(m_bDrawMesh, m_bDrawSkeleton);
    pRenderNode->setT(m_tValue);


    m_ProjectTwo.tick();
    m_ProjectThree.tick();
    m_ProjectFour.tick();

    extras();

    ImGui::End();
    
}

void ProjectOneGameObject::shutdown()
{
    m_ProjectTwo.shutdown();
    m_ProjectThree.shutdown(); 
    m_ProjectFour.shutdown();
}

void ProjectOneGameObject::setAnimation(Animation* pAnimation)
{
    m_currentAnimation = pAnimation;
    m_tValue = 0.0f;
    m_bPlaying = false;

    RenderSkinnedMeshNode* pRenderNode = dynamic_cast<RenderSkinnedMeshNode*>(m_pRenderNode);
    if (pRenderNode == nullptr) {
        assert(0);
        return;
    }

    if (m_currentAnimation == nullptr) {
        pRenderNode->bindPose();
    }
    else {
        pRenderNode->setAnimation(m_currentAnimation);

        m_playbackSpeed = m_currentAnimation->ticksPerSecond / m_currentAnimation->duration;
    }

}

void ProjectOneGameObject::extras()
{
    ImGui::NewLine();
    ImGui::Checkbox("Extras", &m_extrasActive);
    if (m_extrasActive) {

        if (ImGui::Checkbox("Enable WASD Movement", &m_bEnableWASDMovement));
        ImGui::Checkbox("Animated Movement", &m_bMovementControlAnimations);
        ImGui::Checkbox("Camera Follow Player", &m_bCameraFollowPlayer);
        ImGui::SliderFloat("Movement Speed", &m_movementSpeed, 1.0f, 25.0f);
        ImGui::Text("Dance Key: F, Kick: C, KickUp: X");

    }

    if (m_bEnableWASDMovement) {
        Vector3 movement{ 0.0f,0.0f,0.0f };

        Camera& camera = EngineFunctions::Camera();

        bool bMovedThisFrame{ false };
        if (Input::KeyDown(Key::W)) {
            movement[2] += -1.0f;
        }
        if (Input::KeyDown(Key::S)) {
            movement[2] += 1.0f;
        }
        if (Input::KeyDown(Key::A)) {
            movement[0] += -1.0f;
        }
        if (Input::KeyDown(Key::D)) {
            movement[0] += 1.0f;
        }
        if (movement.magnitude_squared() > 0.0f) {
            bMovedThisFrame = true;
        }
        Vector3 forward = camera.forward();
        forward[1] = 0.0f;
        forward.normalize_equal();

        Vector3 right = camera.right();
        right[1] = 0.0f;
        right.normalize_equal();

        movement = movement[0] * right + movement[2] * forward;

        movement.normalize_equal();
        getTransform().position += movement * m_movementSpeed * EngineFunctions::delta_time();
        
        if (m_bCameraFollowPlayer) {
            CameraController::s_instance->setPosition(getTransform().position);
        }

        if (bMovedThisFrame) {
            lookDir(movement);
        }

        if (m_bMovementControlAnimations) {


            if (Input::KeyDown(Key::F) && !bMovedThisFrame) {
                if (m_currentAnimation != m_pAnimDance) {
                    setAnimation(m_pAnimDance);
                    m_bPlaying = true;
                }
            }
            else {
                if (m_currentAnimation == m_pAnimDance) {
                    setAnimation(m_pAnimIdle);
                    m_bPlaying = true;
                }
            }

            Vector3 comDir = m_ProjectFour.m_centerOfMass - getTransform().position;
            comDir.normalize_equal();

            if (Input::KeyClicked(Key::Z)) {

                m_ProjectFour.OnCarryButton();
            }
            bool bCarryingBox = m_ProjectFour.m_bCarryBox;


            if (Input::KeyClicked(Key::X) && !bCarryingBox) {
                setAnimation(m_pAnimKickUp);
                m_playbackSpeed *= 2.0f;
                m_bPlaying = true;
                lookDir(comDir);
            }
            if (Input::KeyClicked(Key::C) && !bCarryingBox) {
                setAnimation(m_pAnimKick);
                m_playbackSpeed *= 2.0f;
                m_bPlaying = true;
                lookDir(comDir);
            }

            if (Input::KeyClicked(Key::C) || Input::KeyClicked(Key::X) && bCarryingBox) {
                m_ProjectFour.ThrowBox();
            }

            if (bMovedThisFrame && !m_bMovedLastFrame) {
                setAnimation(m_pAnimWalk);
                m_bPlaying = true;
            }
            if (!bMovedThisFrame && m_bMovedLastFrame) {
                setAnimation(m_pAnimIdle);
                m_bPlaying = true;
            }

            if (m_currentAnimation == nullptr) {
                setAnimation(m_pAnimIdle);
                m_bPlaying = true;
            }
        }

        m_bMovedLastFrame = bMovedThisFrame;

    }
}

void ProjectOneGameObject::resetPosition()
{

    getTransform().position = Vector3(0.0f, -5.5f-12.0f, 0.0f); //position at base
    getTransform().rotation = Quaternion();
}

void ProjectOneGameObject::lookDir(Vector3 const& dir)
{
    float angle = atan2(-dir[2], dir[0]);
    getTransform().rotation = Quaternion(Vector3(0.0f, 1.0f, 0.0f), angle + pi / 2.0f);// Quaternion(Vector3(1.0f, 0.0f, 0.0f), movement);

}
*/
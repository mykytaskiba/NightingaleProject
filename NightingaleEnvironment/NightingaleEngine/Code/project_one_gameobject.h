#pragma once
#include "game_object.h"
#include "animation.h"
#include "project_two_coobject.h"
#include "project_three_coobject.h"
#include "project_four_coobject.h"

//Gameobject for Project1 CS560
class ProjectOneGameObject : public GameObject {
    friend class ProjectTwo;
    friend class ProjectThree;
    friend class ProjectFour; 
protected:
    virtual void init();
    virtual void tick();
    virtual void shutdown();

private:

    bool m_uiActive{ false };

    float m_tValue;
    bool m_bPlaying{ false };
    bool m_bDrawMesh{ true };
    bool m_bDrawSkeleton{ false };
    float m_playbackSpeed;
    
    Animation* m_currentAnimation{ nullptr };
    Animation* m_pAnimWalk{ nullptr };
    Animation* m_pAnimDance{ nullptr };
    Animation* m_pAnimIdle{ nullptr };
    Animation* m_pAnimKickUp{ nullptr };
    Animation* m_pAnimKick{ nullptr };
    Animation* m_pAnimThrow{ nullptr };


    ProjectTwo m_ProjectTwo = ProjectTwo(*this);
    ProjectThree m_ProjectThree = ProjectThree(*this);
    ProjectFour m_ProjectFour = ProjectFour(*this);

    void setAnimation(Animation* pAnimation);

    //such as WASD movement
    void extras();

    bool m_bEnableWASDMovement{ false };
    bool m_bCameraFollowPlayer{ false };
    float m_movementSpeed{ 10.0f };
    bool m_bMovedLastFrame{ false };
    bool m_bMovementControlAnimations{ true };
    bool m_extrasActive{ true };

    void resetPosition();

    void lookDir(Vector3 const& dir);



};
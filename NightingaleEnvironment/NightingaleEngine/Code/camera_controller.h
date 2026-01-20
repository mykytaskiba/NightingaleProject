#pragma once
#include "game_object.h"
#include "ngmath.h"

class CameraController : public GameObject{

protected:

    Matrix4x4 m_rotationX;
    Matrix4x4 m_rotationY;
    float m_rotationSpeed;

    Vector3 m_position;
    float m_moveSpeed;

    float m_zoomSpeed;
    float m_zoom;
    float m_zoomMin;
    float m_zoomMax;

    virtual void init();
    virtual void tick();
    virtual void render(); 
    virtual void shutdown();
public:
    static CameraController* s_instance;
    void setPosition(Vector3);
};
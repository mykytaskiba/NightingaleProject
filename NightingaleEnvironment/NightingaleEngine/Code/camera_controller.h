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

    void init() override;
    void tick(FrameContext& context) override;
public:
    static CameraController* s_instance;
    void setPosition(Vector3);

    CameraController() : GameObject("camera_controller") {}

    JSON_CHILD(CameraController, 1u, GameObject)

    void properties(IPropertyVisitor& visitor) override {
        GameObject::properties(visitor);
        visitor("zoom speed", m_zoomSpeed);
        visitor("zoom", m_zoom);
        visitor("zoomMin", m_zoomMin);
        visitor("zoomMax", m_zoomMax);
    }
};
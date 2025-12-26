#pragma once
#include "matrix.h"
#include "defines.h"

class Screen;

enum class CameraType {
    Custom,
    Perspective,
    Orthographic
};

class Camera {

public:
    Camera();

    Matrix4x4 const& GetPerspectiveMatrix() const;
    Matrix4x4 const& GetViewMatrix() const;
    void SetViewMatrix(Matrix4x4 const& matrix);
    void SetPerspective(float front, float back, float fov);
    void SetTargetSize(uint width, uint height);

    Vector3 forward();
    Vector3 right();

private:

    void RecalculateCachedMatrix() const;
    mutable Matrix4x4 m_cachedPerspectiveMatrix;

    uint m_targetWidth, m_targetHeight;
    CameraType m_type;
    Matrix4x4 m_viewMatrix;
    Matrix4x4 m_perspectiveMatrix;
};
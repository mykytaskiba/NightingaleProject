#pragma once
#include "defines.h"
#include "ngmath.h"

class Screen;

enum class CameraType {
    Custom,
    Perspective,
    Orthographic
};

class Camera {

public:
    Camera() = default;

    Matrix4x4 const& GetPerspectiveMatrix() const;
    Matrix4x4 const& GetViewMatrix() const;
    void SetViewMatrix(Matrix4x4 const& matrix);
    void SetPerspective(float front, float back, float fov);
    void SetTargetSize(uint width, uint height);

    Vector3 const& forward() const;
    Vector3 const& right() const;

private:

    void RecalculateCachedMatrix() const;
    mutable Matrix4x4 m_cachedPerspectiveMatrix{Matrix4x4::identity()};

    uint m_targetWidth{ 0 }, m_targetHeight{ 0 };
    CameraType m_type{CameraType::Custom};
    Matrix4x4 m_viewMatrix{Matrix4x4::identity()};
    Matrix4x4 m_perspectiveMatrix{Matrix4x4::identity()};
};
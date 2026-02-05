#include "camera.h"



Matrix4x4 const& Camera::GetPerspectiveMatrix() const
{
    return m_cachedPerspectiveMatrix;
}

Matrix4x4 const& Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}


void Camera::SetViewMatrix(Matrix4x4 const& matrix)
{
    m_viewMatrix = matrix;
}

void Camera::SetPerspective(float front, float back, float fov)
{
    m_perspectiveMatrix.make_perspective(front, back, fov); 
    m_type = CameraType::Perspective;
    RecalculateCachedMatrix();
}

void Camera::SetTargetSize(uint width, uint height)
{
    m_targetWidth = width;
    m_targetHeight = height;
    RecalculateCachedMatrix();
}

Vector3 const& Camera::forward() const
{
    //TO DO: return a ref, make truncate dimension return a ref?
    return m_viewMatrix[2].truncate_dimension();
}

Vector3 const& Camera::right() const
{
    //TO DO: return a ref, make truncate dimension return a ref?
    return m_viewMatrix[0].truncate_dimension();
}

void Camera::RecalculateCachedMatrix() const
{
    m_cachedPerspectiveMatrix = m_perspectiveMatrix;

    if (m_type == CameraType::Perspective) {
        if (m_targetWidth == 0 || m_targetHeight == 0) return; //not valid screen size for matrix
        float aspectINV = ((float)m_targetHeight) / ((float)m_targetWidth); //W/H is normal aspect, so H/W is inverse

        m_cachedPerspectiveMatrix[0][0] *= aspectINV;
    }
}



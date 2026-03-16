#include "pch.h"
#include "camera_controller.h"
#include "engine_functions.h"
#include "input.h"
#include "ngrender.h"

CameraController* CameraController::s_instance = nullptr;

void CameraController::init()
{
    s_instance = this;

    m_rotationX.make_identity();
    m_rotationY.make_identity();

    m_rotationSpeed = 0.2f;

    m_zoom = 30.0f;
    m_zoomSpeed = 5.0f;
    m_zoomMin = 3.0f;
    m_zoomMax = 50.0f;

    m_position = Vector3(0.0f, 0.0f, 0.0f);
    m_moveSpeed = 3.0f;

    EngineFunctions::Camera().SetPerspective(0.1f, 5000.0f, 1.2f);

}

void CameraController::tick()
{

    if (Input::MouseDown(MouseButton::Right)) {
        Matrix4x4 rotationDeltaY; 
        rotationDeltaY.make_rotation_y(m_rotationSpeed * EngineFunctions::delta_time() * Input::MouseDelta()[0]);
        Matrix4x4 rotationDeltaX;
        rotationDeltaX.make_rotation_x(m_rotationSpeed * EngineFunctions::delta_time() * Input::MouseDelta()[1]);
        m_rotationY = rotationDeltaY * m_rotationY;
        m_rotationX = rotationDeltaX * m_rotationX;

    }
    Matrix4x4 rotationMatrix = m_rotationX * m_rotationY;

    m_zoom += -m_zoomSpeed * Input::MouseScroll();
    if (m_zoom < m_zoomMin) {
        m_zoom = m_zoomMin;
    }
    if (m_zoom > m_zoomMax) {
        m_zoom = m_zoomMax;
    }
    Matrix4x4 zoomMatrix;
    zoomMatrix.make_translation(0.0f, 0.0f, -m_zoom);

    if (Input::MouseDown(MouseButton::Left) || Input::MouseDown(MouseButton::Middle)) {
        if (Input::KeyDown(Key::LShift)) {
            Matrix4x4 transposed = rotationMatrix.transpose();
            Vector3 forward = transposed[2].truncate_dimension().normalized();
            Vector3 right = transposed[0].truncate_dimension().normalized();
            Vector3 moveDelta =
                forward * Input::MouseDelta()[1] +
                right * Input::MouseDelta()[0];
            moveDelta *= EngineFunctions::delta_time();
            m_position += moveDelta;
        }
    }
    Matrix4x4 translationMatrix;
    translationMatrix.make_translation(-1.0f * m_position);


    Matrix4x4 viewMatrix = zoomMatrix * rotationMatrix * translationMatrix;
    EngineFunctions::Camera().SetViewMatrix(viewMatrix);
}

void CameraController::render()
{
}

void CameraController::shutdown()
{
}

void CameraController::setPosition(Vector3 vec)
{
    m_position[0] = -vec[0];
    m_position[2] = -vec[2];
}

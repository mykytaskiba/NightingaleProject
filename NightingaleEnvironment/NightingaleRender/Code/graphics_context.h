#pragma once
#include "ngmath.h"

class RenderShader;
class Mesh;
class Camera;


struct GraphicsContext {
private:
    bool m_bDirtyShader{ true };
    RenderShader* m_pCurrentShader{ nullptr };

    bool m_bDirtyCamera{ true };
    Camera* m_pCamera{ nullptr };
    
    unsigned int m_targetWidth{ 0u };
    unsigned int m_targetHeight{ 0u };
    
    static constexpr Color s_defaultColor = Color{ 1.0f, 1.0f, 0.0f, 1.0f };
public:
    
    unsigned int targetWidth() { return m_targetWidth; }
    unsigned int targetHeight() { return m_targetHeight; }

    void setTargetDimensions(unsigned int width, unsigned int height) {
        m_targetWidth = width; m_targetHeight = height;
    }
    
    void setCamera(Camera* pCamera);
    void setCurrentShader(RenderShader*);
    RenderShader* getCurrentShader();
    

    void beginFrame();
    void endFrame();
};
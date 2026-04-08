#pragma once
#include "ngmath.h"

class RenderShader;
class Mesh;
class Camera;

struct TestGraphicsConstants {

    unsigned int m_lineVAO;
    RenderShader* m_pLineShader;

    RenderShader* m_pUnlitShader;
    Mesh* m_pSphere;
    Mesh* m_pCube;
};

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
    
    void setCamera(Camera* pCamera);
    void setCurrentShader(RenderShader*);
    RenderShader* getCurrentShader();
    
    uint m_lineVAO;
    RenderShader* m_pLineShader;
    
    RenderShader* m_pUnlitShader;
    Mesh* m_pSphere;
    Mesh* m_pCube;


    void init();


    void drawAxisAlignedBox(AxisAlignedBox const& box, Color const& color = s_defaultColor);

    void drawLine(Vector3 const& from, Vector3 const& to, Color const& color = s_defaultColor);
    void drawSphere(Vector3 const& at, float scale = 1.0f, Color const& color = s_defaultColor);
    void drawCube(Vector3 const& center, Vector3 const& scale, Color const& color = s_defaultColor);

    void drawWireSphere(Vector3 const& at, float scale = 1.0f, Color const& color = s_defaultColor);
    void drawWireCube(Vector3 const& center, Vector3 const& scale, Color const& color = s_defaultColor);

    void endFrame();
};
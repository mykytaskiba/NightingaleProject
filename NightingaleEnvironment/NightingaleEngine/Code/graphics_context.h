#pragma once
#include "ngmath.h"
#include "screen.h"

class RenderShader;
class Mesh;

struct GraphicsContext {
private:
    RenderShader* m_pCurrentShader{ nullptr };

public:
    //TO DO: possibly refactor this?
    Screen m_currentScreenDims{};

    void setCurrentShader(RenderShader*);
    RenderShader* getCurrentShader();
    
    uint m_lineVAO;
    RenderShader* m_pLineShader;
    
    RenderShader* m_pUnlitShader;
    Mesh* m_pSphere;
    Mesh* m_pCube;


    void init();

    static constexpr Color s_defaultColor = Color{ 1.0f, 1.0f, 0.0f, 1.0f };

    void drawAxisAlignedBox(AxisAlignedBox const& box, Color const& color = s_defaultColor);

    void drawLine(Vector3 const& from, Vector3 const& to, Color const& color = s_defaultColor);
    void drawSphere(Vector3 const& at, float scale = 1.0f, Color const& color = s_defaultColor);
    void drawCube(Vector3 const& center, Vector3 const& scale, Color const& color = s_defaultColor);

    void drawWireSphere(Vector3 const& at, float scale = 1.0f, Color const& color = s_defaultColor);
    void drawWireCube(Vector3 const& center, Vector3 const& scale, Color const& color = s_defaultColor);

    void endFrame();
};
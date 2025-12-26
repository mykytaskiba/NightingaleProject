#pragma once
#include "vector.h"
#include "color.h"

class RenderShader;
class Mesh;

struct GraphicsContext {
private:
    RenderShader* m_pCurrentShader{ nullptr };

public:
    void setCurrentShader(RenderShader*);
    RenderShader* getCurrentShader();
    
    uint m_lineVAO;
    RenderShader* m_pLineShader;
    
    RenderShader* m_pUnlitShader;
    Mesh* m_pSphere;

    void init();

    void drawLine(Vector3 const& from, Vector3 const& to, Color const& color = Color(1.0f,1.0f,0.0f,1.0f));
    void drawSphere(Vector3 const& at, float scale = 1.0f, Color const& color = Color(1.0f, 1.0f, 0.0f, 1.0f));

    void endFrame();
};
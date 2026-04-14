#pragma once
#include "color.h"
#include "vector.h"
#include "ngmath.h"

class RenderShader;
class Mesh;
class GraphicsContext;


struct RenderPrimitives {
    GraphicsContext& m_context;
    RenderPrimitives(GraphicsContext& context);

    unsigned int m_lineVAO{ 0 };
    RenderShader* m_pLineShader{ nullptr };
    RenderShader* m_pUnlitShader{ nullptr };
    Mesh* m_pSphere{ nullptr };
    Mesh* m_pCube{ nullptr };

    static constexpr Color s_defaultColor = Color{ 1.0f,1.0f,0.0f,1.0f };

    void setLineShader(RenderShader* pLineShader);
    void setUnlitShader(RenderShader* pUnlitShader);
    void setSphereMesh(Mesh* pSphere);
    void setCubeMesh(Mesh* pCube);

    void drawAxisAlignedBox(AxisAlignedBox const& box, Color const& color = s_defaultColor) const;

    void drawLine(Vector3 const& from, Vector3 const& to, Color const& color = s_defaultColor) const;
    void drawSphere(Vector3 const& at, float scale = 1.0f, Color const& color = s_defaultColor) const;
    void drawCube(Vector3 const& center, Vector3 const& scale, Color const& color = s_defaultColor) const;

    void drawWireSphere(Vector3 const& at, float scale = 1.0f, Color const& color = s_defaultColor) const;
    void drawWireCube(Vector3 const& center, Vector3 const& scale, Color const& color = s_defaultColor) const;
};

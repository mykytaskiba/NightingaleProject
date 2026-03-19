#pragma once
#include "render_node.h"
#include "graphics_context.h"
#include "ngmath.h"

class Mesh;
class RenderShader;
class GraphicsContext;

class RenderMeshNode : public RenderNode {
public:
    void init(GameObject* pGameObject) override;
    void render(GraphicsContext& context) override;

    void setMesh(Mesh* pMesh);

    Color const& getColor() const { return m_color; }
    void setColor(Color const& color) { m_color = color; }

private:
    Mesh* m_pMesh{ nullptr };
    RenderShader* m_pShader{ nullptr };
    GameObject* m_pGameObject{ nullptr };
    Color m_color{ 1.0f,1.0f,1.0f,1.0f}; //White by default
};
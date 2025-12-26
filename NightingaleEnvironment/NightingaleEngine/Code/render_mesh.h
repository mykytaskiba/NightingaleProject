#pragma once
#include "render_node.h"
#include "graphics_context.h"
#include "color.h"

class Mesh;
class RenderShader;
class GraphicsContext;

class RenderMeshNode : public RenderNode {
public:
    void init(GameObject* pGameObject) override;
    void render(GraphicsContext& context) override;

    void setMesh(Mesh* pMesh);
    void setColor(Color const& color);
private:
    Mesh* m_pMesh;
    RenderShader* m_pShader;
    GameObject* m_pGameObject;
    Color m_color{ 1.0f,1.0f,1.0f,1.0f}; //White by default
};
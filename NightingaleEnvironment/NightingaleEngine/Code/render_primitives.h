#pragma once

class RenderShader;
class Mesh;

struct RenderPrimitives {

    unsigned int m_lineVAO;
    RenderShader* m_pLineShader;

    RenderShader* m_pUnlitShader;
    Mesh* m_pSphere;
    Mesh* m_pCube;

    void init();


    //void drawAxisAlignedBox(AxisAlignedBox const& box, Color const& color = s_defaultColor);

    //void drawLine(Vector3 const& from, Vector3 const& to, Color const& color = s_defaultColor);
    //void drawSphere(Vector3 const& at, float scale = 1.0f, Color const& color = s_defaultColor);
    //void drawCube(Vector3 const& center, Vector3 const& scale, Color const& color = s_defaultColor);

    //void drawWireSphere(Vector3 const& at, float scale = 1.0f, Color const& color = s_defaultColor);
    //void drawWireCube(Vector3 const& center, Vector3 const& scale, Color const& color = s_defaultColor);
};

/*


void GraphicsContext::drawAxisAlignedBox(AxisAlignedBox const& box, Color const& color)
{
    Vector3 min = box.min();
    Vector3 max = box.max();
    Vector3 extends = box.extents();
    Vector3 right = { extends[0],0,0 };
    Vector3 up = { 0,extends[1],0 };
    Vector3 forward = { 0,0,extends[2] };

    drawLine(min, min + up, color);
    drawLine(min + right, min + up + right, color);
    drawLine(min + forward, min + up + forward, color);
    drawLine(max, max - up, color);

    drawLine(min, min + right, color);
    drawLine(min, min + forward, color);
    drawLine(max - up, max - up - forward, color);
    drawLine(max - up, max - up - right, color);

    drawLine(max, max - right, color);
    drawLine(max, max - forward, color);
    drawLine(min + up, min + up + right, color);
    drawLine(min + up, min + up + forward, color);

}

void GraphicsContext::drawLine(Vector3 const& from, Vector3 const& to, Color const& color)
{
    setCurrentShader(m_pLineShader);

    m_pLineShader->transferByName("uFromPoint", from);
    m_pLineShader->transferByName("uDestPoint", to);

    m_pLineShader->transferByName("uColor", color);

    glBindVertexArray(m_lineVAO);
    glDrawArrays(GL_LINES, 0, 2);
}

void GraphicsContext::drawSphere(Vector3 const& at, float scale, Color const& color)
{
    setCurrentShader(m_pUnlitShader);

    Transform transform;
    transform.scale = Vector3(scale, scale, scale);
    transform.position = at;

    m_pUnlitShader->transferByName("uModelMatrix", transform.getMatrix());
    m_pUnlitShader->transferByName("uColor", color);
    m_pSphere->Draw();

}

void GraphicsContext::drawCube(Vector3 const& center, Vector3 const& scale, Color const& color)
{
    setCurrentShader(m_pUnlitShader);

    Transform transform;
    transform.position = center;

    Matrix4x4 scaleMatrix;
    scaleMatrix.make_scale(scale);
    Matrix4x4 transformMatrix = transform.getMatrix() * scaleMatrix;

    m_pUnlitShader->transferByName("uModelMatrix", transformMatrix);
    m_pUnlitShader->transferByName("uColor", color);
    m_pCube->Draw();
}

void GraphicsContext::drawWireSphere(Vector3 const& at, float scale, Color const& color)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    drawSphere(at, scale, color);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GraphicsContext::drawWireCube(Vector3 const& center, Vector3 const& scale, Color const& color)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    drawCube(center, scale, color);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
*/
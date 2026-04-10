#include "mesh.h"
#include "GL/glew.h"

void Mesh::Draw()
{
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_triangleCount, GL_UNSIGNED_INT, 0);
}

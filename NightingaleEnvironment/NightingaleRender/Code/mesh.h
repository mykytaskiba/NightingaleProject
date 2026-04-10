#pragma once

class Mesh {

    friend class MeshBuilder;

private:
    unsigned int m_VAO;
    unsigned int m_triangleCount;
    //TO DO: Make this private
    Mesh(unsigned int VAO, unsigned int triangleCount) : m_VAO(VAO), m_triangleCount(triangleCount) { }

public:

    void Draw();

};



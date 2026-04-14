#pragma once
#include "ngmath.h"
#include "render_primitives.h"

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
 
    RenderPrimitives m_primitives{};

public:
    
    unsigned int targetWidth() { return m_targetWidth; }
    unsigned int targetHeight() { return m_targetHeight; }

    void setTargetDimensions(unsigned int width, unsigned int height) {
        m_targetWidth = width; m_targetHeight = height;
    }
    
    void setCamera(Camera* pCamera);
    void setCurrentShader(RenderShader*);
    RenderShader* getCurrentShader();
    
    void draw(Mesh* pMesh);

    //Check for any dirty state and update
    //return true if ready to draw
    bool updateDirtyState();

    void beginFrame();
    void endFrame();
};
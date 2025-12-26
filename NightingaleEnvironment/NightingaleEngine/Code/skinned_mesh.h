#pragma once

class Mesh;
class Skeleton;
//Connection to connect a mesh and a skeleton
//this is responsible for calculating bone matricies from the skeleton
class SkinnedMesh {


    Mesh* m_pMesh;
    Skeleton* m_pSkeleton;

public:
    void Init(Mesh*, Skeleton*);

};
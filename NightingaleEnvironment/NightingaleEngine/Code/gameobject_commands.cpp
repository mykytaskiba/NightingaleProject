#include "pch.h"
#include "gameobject_commands.h"
#include "game_object.h"
#include "gameobject_factory.h"
#include "mesh.h"
#include "asset_manager.h"
#include "render_mesh.h"
#include "engine_functions.h"
#include "skeleton.h"
#include "render_skinned.h"
#include "color.h"

/*
ExecutionResult CreateGameObjectCommand::execute(string args, ExecutionState& state)
{
    string type = nextArg(args);
    string rest = nextArg(args);
    
    string expected = m_command + " (type)";

    ExecutionResult result;
    result.bSuccess = false;
    if (rest != "") {
        result.message = "too many arguments, expected: " + expected;
        return result;
    }
    if (type == "") {
        result.message = "too few arguments, expected: " + expected;
        return result;
    }

    GameObject* pCreatedObject = nullptr;
   
    if (!GameObjectFactory::instantiateFromType(type, pCreatedObject)) {
        result.message = "failed instantiating object of type " + type;
        return result;
    }

    state.pGameObject = pCreatedObject;

    result.bSuccess = true;
    result.message = "created an object of type " + type;
    return result;
}

ExecutionResult AttachRenderMeshCommand::execute(string args, ExecutionState& state)
{
    string meshName = nextArg(args);

    ExecutionResult result;
    result.bSuccess = false;

    if (meshName == "") {
        result.message = "Must specify MeshName from asset loader";
        return result;
    }

    if (!AssetManager<Mesh>::has(meshName)) {
        result.message = "Mesh is not loaded: " + meshName;
        return result;
    }

    if (state.pGameObject == nullptr) {
        result.message = "Invalid state, previous gameobject must be set";
        return result;
    }

    Mesh* pMesh = AssetManager<Mesh>::retrieve(meshName);

    RenderMeshNode* pMeshNode = new RenderMeshNode();

    pMeshNode->setMesh(pMesh);

    EngineFunctions::AssignRenderNode(state.pGameObject, pMeshNode);
    
    state.pRenderNode = pMeshNode;

    result.bSuccess = true;
    result.message = "AttachMesh success";
    return result;
}

ExecutionResult SetPositionCommand::execute(string args, ExecutionState& state)
{
    ExecutionResult result;
    if (state.pGameObject == nullptr) {
        result.message = "Invalid state, previous gameobject must be set";
    }

    string expected = "setposition (x) (y) (z)";
    result.message = "failed to parse float, expected: " + expected; 
    float x, y, z;
    if (!nextFloat(args, x)) {
        return result;
    }
    if (!nextFloat(args, y)) {
        return result;
    }
    if (!nextFloat(args, z)) {
        return result;
    }

    state.pGameObject->getTransform().position = Vector3(x, y, z);
    
    result.bSuccess = true;
    result.message = "position has been set";

    return result;
}

ExecutionResult SetScaleCommand::execute(string args, ExecutionState& state)
{
    ExecutionResult result;
    if (state.pGameObject == nullptr) {
        result.message = "Invalid state, previous gameobject must be set";
    }

    string expected = "setscale (scale)";
    result.message = "failed to parse float, expected: " + expected;
    float scale, y, z;
    if (!nextFloat(args, scale)) {
        return result;
    }

    state.pGameObject->getTransform().scale = scale;

    result.bSuccess = true;
    result.message = "scale has been set";

    return result;

}

ExecutionResult AttachAnimatedMeshCommand::execute(string args, ExecutionState& state)
{
    string skeletonName = nextArg(args);

    ExecutionResult result;
    result.bSuccess = false;

    if (skeletonName == "") {
        result.message = "Must specify SkeletonName from asset loader";
        return result;
    }

    if (!AssetManager<Skeleton>::has(skeletonName)) {
        result.message = "Skeleton is not loaded: " + skeletonName;
        return result;
    }

    if (state.pGameObject == nullptr) {
        result.message = "Invalid state, previous gameobject must be set";
        return result;
    }

    Skeleton* pSkeleton = AssetManager<Skeleton>::retrieve(skeletonName);

    RenderSkinnedMeshNode* pRenderNode = new RenderSkinnedMeshNode();

    pRenderNode->setSkeleton(pSkeleton);

    EngineFunctions::AssignRenderNode(state.pGameObject, pRenderNode);

    state.pRenderNode = pRenderNode;

    result.bSuccess = true;
    result.message = "Attach AnimatedMesh success";
    return result;
}

ExecutionResult SetColorCommand::execute(string args, ExecutionState& state)
{
    ExecutionResult result;
    if (state.pRenderNode == nullptr) {
        result.message = "Invalid state, previous renderNode must be set";
    }

    RenderMeshNode* pRenderMesh = dynamic_cast<RenderMeshNode*>(state.pRenderNode);
    if (pRenderMesh == nullptr) {
        result.message = "Invalid state, currently only renderMesh is supported";

    }

    string expected = "setcolor (x) (y) (z)";
    result.message = "failed to parse float, expected: " + expected;
    float r, g, b;
    if (!nextFloat(args, r)) {
        return result;
    }
    if (!nextFloat(args, g)) {
        return result;
    }
    if (!nextFloat(args, b)) {
        return result;
    }

    Color color{ r,g,b, 1.0f };
    
    pRenderMesh->setColor(color);

    result.bSuccess = true;
    result.message = "position has been set";

    return result;
}
*/
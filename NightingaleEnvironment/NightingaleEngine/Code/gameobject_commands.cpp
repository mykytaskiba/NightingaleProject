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
#include "ngmath.h"

void CreateGameObjectCommand::execute_command(ArgumentList<Line>& args, ExecutionState& state, ExecutionResult& result)
{
    string type = *args.get<0>();
    
    GameObject* pCreatedObject = nullptr;

    if (!GameObjectFactory::instantiateFromType(type, pCreatedObject)) {
        result.message = "failed instantiating object of type " + type;
        result.bSuccess = false;
        return;
    }

    state.pGameObject = pCreatedObject;

    result.bSuccess = true;
    result.message = "created an object of type " + type;
    return;
}

void AttachRenderMeshCommand::execute_command(ArgumentList<Line>& args, ExecutionState& state, ExecutionResult& result)
{
    string meshName = *args.get<0>();

    if (!AssetManager<Mesh>::has(meshName)) {
        result.bSuccess = false;
        result.message = "Mesh is not loaded: " + meshName;
        return;
    }

    if (state.pGameObject == nullptr) {
        result.bSuccess = false;
        result.message = "Invalid state, previous gameobject must be set";
        return;
    }

    Mesh* pMesh = AssetManager<Mesh>::retrieve(meshName);

    RenderMeshNode* pMeshNode = new RenderMeshNode();

    pMeshNode->setMesh(pMesh);

    EngineFunctions::AssignRenderNode(state.pGameObject, pMeshNode);

    state.pRenderNode = pMeshNode;

    result.bSuccess = true;
    result.message = "AttachMesh success";
    return;
}

void AttachAnimatedMeshCommand::execute_command(ArgumentList<Line>& args, ExecutionState& state, ExecutionResult& result)
{
    string skeletonName = *args.get<0>();



    if (!AssetManager<Skeleton>::has(skeletonName)) {
        result.bSuccess = false;
        result.message = "Skeleton is not loaded: " + skeletonName;
        return;
    }

    if (state.pGameObject == nullptr) {
        result.bSuccess = false;
        result.message = "Invalid state, previous gameobject must be set";
        return;
    }

    Skeleton* pSkeleton = AssetManager<Skeleton>::retrieve(skeletonName);

    RenderSkinnedMeshNode* pRenderNode = new RenderSkinnedMeshNode();

    pRenderNode->setSkeleton(pSkeleton);

    EngineFunctions::AssignRenderNode(state.pGameObject, pRenderNode);

    state.pRenderNode = pRenderNode;

    result.bSuccess = true;
    result.message = "Attach AnimatedMesh success";
    return;
}

void SetPositionCommand::execute_command(ArgumentList<Vector3>& args, ExecutionState& state, ExecutionResult& result)
{
    if (state.pGameObject == nullptr) {
        result.message = "Invalid state, previous gameobject must be set";
        return;
    }

    state.pGameObject->getTransform().position = args.get<0>();

    result.bSuccess = true;
    result.message = "position has been set";

    return;
}

void SetScaleCommand::execute_command(ArgumentList<float>& args, ExecutionState& state, ExecutionResult& result)
{
    if (state.pGameObject == nullptr) {
        result.message = "Invalid state, previous gameobject must be set";
        return;
    }

    float scale = args.get<0>();
    state.pGameObject->getTransform().scale = scale;

    result.bSuccess = true;
    result.message = "scale has been set";

    return;
}

void SetColorCommand::execute_command(ArgumentList<float, float, float>& args, ExecutionState& state, ExecutionResult& result)
{
    if (state.pRenderNode == nullptr) {
        result.message = "Invalid state, previous renderNode must be set";
        return;
    }

    RenderMeshNode* pRenderMesh = dynamic_cast<RenderMeshNode*>(state.pRenderNode);
    if (pRenderMesh == nullptr) {
        result.message = "Invalid state, currently only renderMesh is supported";
        return;
    }

    float r = args.get<0>();
    float g = args.get<1>();
    float b = args.get<2>();

    Color color{ r,g,b, 1.0f };

    pRenderMesh->setColor(color);

    result.bSuccess = true;
    result.message = "color has been set";

    return;
}

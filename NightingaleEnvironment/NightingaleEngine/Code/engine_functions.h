#pragma once
#include "defines.h"
#include "scripting.h"

class GameObject;
class Camera;
class RenderNode;
class CallbackHandler;

class EngineFunctions {
    friend struct SceneChangePackage;
private:
    static void Setup(GameObject* setup);

    //to do rename
    static void PreSetup(GameObject* setup);

    static void SetParent(GameObject* pGameObject, GameObject* pParent);

public:
    
    template <typename TGameObject>
    static TGameObject* InstantiateGameObject() {
        TGameObject* pGameObject = new TGameObject();
        PreSetup(pGameObject);
        return pGameObject;
    }

    static void AssignRenderNode(GameObject* pGameObject, RenderNode* pRenderNode);

    static Camera& Camera();

    static int ScreenWidth();
    static int ScreenHeight();

    static TTimeDelta DeltaTime();

    static ExecutionResult ExecuteCommand(string const&);

    static void SetWindowTitle(string const& title);
    static void SetTargetFramerate(int framerate);

    static CallbackHandler& getUpdateCallback();

};
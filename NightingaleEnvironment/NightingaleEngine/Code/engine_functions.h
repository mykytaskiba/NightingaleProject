#pragma once
#include "defines.h"
#include "scripting.h"

class GameObject;
class Camera;
class RenderNode;
class CallbackHandler;
class Renderer;
class Physics;
class HotkeyManager;
class Scene;

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

    static void AttachPhysicsBody(GameObject* pGameObject);

    static Camera& Camera();

    static int ScreenWidth();
    static int ScreenHeight();

    static TTimeDelta delta_time();

    //Commands
    static ExecutionResult ExecuteCommand(string const&);

    static void SetWindowTitle(string const& title);
    static void SetTargetFramerate(int framerate);
    static float getTargetFramerate();
    static uint getTargetFramerateInt();

    static CallbackHandler& getUpdateCallback();

    static Renderer& Renderer();
    static Physics& physics();
    static Scene& scene();

    static ExecutionState& execution_state();

    static HotkeyManager& hotkeyManager();

    static void close_app();

};
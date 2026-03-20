#pragma once
#include "defines.h"
#include "command.h"


class CreateGameObjectCommand : public Command<SelectedGameObject, Line> {
public:
    CreateGameObjectCommand() : Command("create_gameobject") {}
    virtual void execute_command(ArgumentList<SelectedGameObject, Line>& args, ExecutionState& state, ExecutionResult& result) override;
};

//attach a single rendermesh to the game object, mesh must be in the AssetManager already
class AttachRenderMeshCommand : public Command<Line> {
public:
    AttachRenderMeshCommand() : Command("attach_rendermesh") {}
    virtual void execute_command(ArgumentList<Line>& args, ExecutionState& state, ExecutionResult& result) override;

};

class AttachAnimatedMeshCommand : public Command<Line> {
public:
    AttachAnimatedMeshCommand() : Command("attach_animatedmesh") {}
    virtual void execute_command(ArgumentList<Line>& args, ExecutionState& state, ExecutionResult& result) override;
};

class SetPositionCommand : public Command <Vector3> {
public:
    SetPositionCommand() : Command("setposition") {}
    virtual void execute_command(ArgumentList<Vector3>& args, ExecutionState& state, ExecutionResult& result) override;
};

class SetScaleCommand : public Command <float> {
public:
    SetScaleCommand() : Command("setscale") {}
    virtual void execute_command(ArgumentList<float>& args, ExecutionState& state, ExecutionResult& result) override;
};

class SetColorCommand : public Command <float, float, float> {
public:
    SetColorCommand() : Command("setcolor") {}
    virtual void execute_command(ArgumentList<float, float, float>& args, ExecutionState& state, ExecutionResult& result) override;
};

//attach a single rendermesh to the game object, mesh must be in the AssetManager already
class DeleteGameObjectCommand : public Command<SelectedGameObject> {
    public:
        DeleteGameObjectCommand() : Command("delete_gameobject") {}
        virtual void execute_command(ArgumentList<SelectedGameObject>& args, ExecutionState& state, ExecutionResult& result) override;
};

class ClearSceneCommand : public Command<Nothing> {
public:
    ClearSceneCommand() : Command("clear_scene") {}
    virtual void execute_command(ArgumentList<Nothing>& args, ExecutionState& state, ExecutionResult& result) override;
};
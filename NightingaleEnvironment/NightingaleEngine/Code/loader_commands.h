#pragma once
#include "defines.h"
#include "command.h"
#include "afterparty.h"

class ShaderLoadCommand : public Command<Line,Line,Line> {
public:
    ShaderLoadCommand() : Command("load_shader") {}
    virtual void execute_command(ArgumentList<Line,Line,Line>& args, ExecutionState& state, ExecutionResult& result) override;
};

class MeshLoadCommand : public Command<Line, Line> {
public:
    MeshLoadCommand() : Command("load_mesh") {}
    virtual void execute_command(ArgumentList<Line, Line>& args, ExecutionState& state, ExecutionResult& result) override;
};

class SkeletonLoadCommand : public Command<Line, Line> {
public:
    SkeletonLoadCommand() : Command("load_skeleton") {}
    virtual void execute_command(ArgumentList<Line, Line>& args, ExecutionState& state, ExecutionResult& result) override;
};

class AnimationLoadCommand : public Command<Line, Line> {
public:
    AnimationLoadCommand() : Command("load_animation") {}
    virtual void execute_command(ArgumentList<Line, Line>& args, ExecutionState& state, ExecutionResult& result) override;

};

class LoaderScaleCommand : public Command<float> {
public:
    LoaderScaleCommand() : Command("set_loader_scale") {}
    virtual void execute_command(ArgumentList<float>& args, ExecutionState& state, ExecutionResult& result) override;

};

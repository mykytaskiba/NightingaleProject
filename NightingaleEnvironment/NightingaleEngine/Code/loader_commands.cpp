#include "pch.h"
#include "loader_commands.h"
#include "core_commands.h"
#include "render_shader.h"
#include "shader_source.h"
#include "loader.h"
#include "engine_functions.h"
#include "asset_manager.h"


void ShaderLoadCommand::execute_command(ArgumentList<Line, Line, Line>& args, ExecutionState& state, ExecutionResult& result)
{
    string shaderName = *args.get<0>();
    string vertexShaderPath = *args.get<1>();
    string fragmentShaderPath = *args.get<2>();

    if (AssetManager<RenderShader>::has(shaderName)) {
        result.message = "shader already exists: " + shaderName;
        result.bSuccess = false;
        return;
    }

    if (!Loader::file_exists(vertexShaderPath)) {
        result.message = "vertex shader not found " + vertexShaderPath;
        result.bSuccess = false;
        return;
    }
    if (!Loader::file_exists(fragmentShaderPath)) {
        result.message = "fragment shader not found " + fragmentShaderPath;
        result.bSuccess = false;
        return;
    }

    string vertexSource = Loader::read_file_contents(vertexShaderPath);
    string fragmentSource = Loader::read_file_contents(fragmentShaderPath);

    GL_Shader GLshader;
    if (!GL_Shader::compileShader(vertexSource.c_str(), fragmentSource.c_str(), GLshader)) {

        result.message = "Shader compilation failed";
        result.bSuccess = false;
        return;
    }

    RenderShader* shader = new RenderShader(GLshader);

    bool success = AssetManager<RenderShader>::add(shaderName, shader);
    assert(success);

    result.message = "Shader compile success";
    result.bSuccess = true;
}

void MeshLoadCommand::execute_command(ArgumentList<Line, Line>& args, ExecutionState& state, ExecutionResult& result)
{
    string meshName = *args.get<0>();
    string pathToMesh = *args.get<1>();

    if (AssetManager<Mesh>::has(meshName)) {
        result.message = "mesh already exists: " + meshName;
        result.bSuccess = false;
        return;
    }

    //TO DO: once loader fbx load is implemented, then this will need to be updated too
    Mesh* pMesh = Loader::fbxSingleMesh(pathToMesh, state.loaderScale);

    bool success = AssetManager<Mesh>::add(meshName, pMesh);
    assert(success);

    result.message = "Mesh load success";
    result.bSuccess = true;
}

void SkeletonLoadCommand::execute_command(ArgumentList<Line, Line>& args, ExecutionState& state, ExecutionResult& result)
{
    string meshName = *args.get<0>();
    string pathToMesh = *args.get<1>();

    if (AssetManager<Skeleton>::has(meshName)) {
        result.message = "mesh already exists: " + meshName;
        result.bSuccess = false;
        return;
    }

    Skeleton* pSkeleton = Loader::fbxSkeleton(pathToMesh, state.loaderScale);

    bool success = AssetManager<Skeleton>::add(meshName, pSkeleton);
    assert(success);

    result.message = "Skeleton load success";
    result.bSuccess = true;
    return;
}

void AnimationLoadCommand::execute_command(ArgumentList<Line, Line>& args, ExecutionState& state, ExecutionResult& result)
{
    string name = *args.get<0>();
    string path = *args.get<1>();

    if (AssetManager<Animation>::has(name)) {
        result.message = "animation already exists: " + name;
        result.bSuccess = false;
        return;
    }

    Animation* pAnimation = Loader::fbxAnimation(path);

    bool success = AssetManager<Animation>::add(name, pAnimation);
    assert(success);

    result.message = "Animation load success";
    result.bSuccess = true;
    return;
}

void LoaderScaleCommand::execute_command(ArgumentList<float>& args, ExecutionState& state, ExecutionResult& result)
{
    state.loaderScale = args.get<0>();
    result.bSuccess = true;
    result.message = "Loader scale set";
    return;
}

#include "pch.h"
#include "loader_commands.h"
#include "core_commands.h"
#include "render_shader.h"
#include "shader_source.h"
#include "loader.h"
#include "engine_functions.h"
#include "asset_manager.h"

/*
ExecutionResult ShaderLoadCommand::execute(string args, ExecutionState& state)
{
    string shaderName = nextArg(args);
    string vertexShaderPath = nextArg(args);
    string fragmentShaderPath = nextArg(args);
    string rest = nextArg(args);

    string expectedCommand = "\"shader_load (shader_name) (path_to_vertex) (path_to_fragment)\"";

    ExecutionResult result;

    if (vertexShaderPath == "" || fragmentShaderPath == "" || shaderName == "") {
        result.message = "too few arguments, expected: " + expectedCommand;
        result.bSuccess = false;
        return result;
    }

    if (rest != "") {
        result.message = "too many arguments, expected: " + expectedCommand;
        result.bSuccess = false;
        return result;
    }

    if (AssetManager<RenderShader>::has(shaderName)) {
        result.message = "shader already exists: " + shaderName;
        result.bSuccess = false;
        return result;
    }

    if (!Loader::file_exists(vertexShaderPath)) {
        result.message = "vertex shader not found " + vertexShaderPath;
        result.bSuccess = false;
        return result;
    }
    if (!Loader::file_exists(fragmentShaderPath)) {
        result.message = "fragment shader not found " + fragmentShaderPath;
        result.bSuccess = false;
        return result;
    }

    string vertexSource = Loader::read_file_contents(vertexShaderPath);
    string fragmentSource = Loader::read_file_contents(fragmentShaderPath);
    
    GL_Shader GLshader;
    if (!GL_Shader::compileShader(vertexSource.c_str(), fragmentSource.c_str(), GLshader)) {

        result.message = "Shader compilation failed";
        result.bSuccess = false;
        return result;
    }

    RenderShader* shader = new RenderShader(GLshader);
    
    bool success = AssetManager<RenderShader>::add(shaderName, shader);
    assert(success);

    result.message = "Shader compile success";
    result.bSuccess = true;
    return result;
}

ExecutionResult MeshLoadCommand::execute(string args, ExecutionState& state)
{
    string meshName = nextArg(args);
    string pathToMesh = nextArg(args);
    string rest = nextArg(args);

    string expectedCommand = "\"mesh_load (mesh_name) (path_to_mesh)\"";

    ExecutionResult result;

    if (meshName == "" || pathToMesh == "") {
        result.message = "too few arguments, expected: " + expectedCommand;
        result.bSuccess = false;
        return result;
    }

    if (rest != "") {
        result.message = "too many arguments, expected: " + expectedCommand;
        result.bSuccess = false;
        return result;
    }

    if (AssetManager<Mesh>::has(meshName)) {
        result.message = "mesh already exists: " + meshName;
        result.bSuccess = false;
        return result;
    }

    //TO DO: once loader fbx load is implemented, then this will need to be updated too
    Mesh* pMesh = Loader::fbxSingleMesh(pathToMesh, state.loaderScale);


    bool success = AssetManager<Mesh>::add(meshName, pMesh);
    assert(success);

    result.message = "Mesh load success";
    result.bSuccess = true;
    return result;
}

ExecutionResult LoaderScaleCommand::execute(string args, ExecutionState& state)
{
    float scale;
    string expected = m_command + " (f)";
    if (!nextFloat(args, scale)) {
        ExecutionResult::FAIL("parsing fail, expected: " + expected);
    }

    state.loaderScale = scale;
    return ExecutionResult::SUCCESS("load scale set");
}

ExecutionResult SkeletonLoadCommand::execute(string args, ExecutionState& state)
{
    string meshName = nextArg(args);
    string pathToMesh = nextArg(args);
    string rest = nextArg(args);

    string expectedCommand = m_command + " (skeleton_name) (path_to_mesh)";

    ExecutionResult result;

    if (meshName == "" || pathToMesh == "") {
        result.message = "too few arguments, expected: " + expectedCommand;
        result.bSuccess = false;
        return result;
    }

    if (rest != "") {
        result.message = "too many arguments, expected: " + expectedCommand;
        result.bSuccess = false;
        return result;
    }

    if (AssetManager<Skeleton>::has(meshName)) {
        result.message = "mesh already exists: " + meshName;
        result.bSuccess = false;
        return result;
    }
    
    Skeleton* pSkeleton = Loader::fbxSkeleton(pathToMesh, state.loaderScale);

    bool success = AssetManager<Skeleton>::add(meshName, pSkeleton);
    assert(success);

    result.message = "Skeleton load success";
    result.bSuccess = true;
    return result;
}

ExecutionResult AnimationLoadCommand::execute(string args, ExecutionState& state)
{
    string name = nextArg(args);
    string path = nextArg(args);
    string rest = nextArg(args);

    string expectedCommand = m_command + " (animation_name) (path_to_fbx)";

    ExecutionResult result;

    if (name == "" || path == "") {
        result.message = "too few arguments, expected: " + expectedCommand;
        result.bSuccess = false;
        return result;
    }

    if (rest != "") {
        result.message = "too many arguments, expected: " + expectedCommand;
        result.bSuccess = false;
        return result;
    }

    if (AssetManager<Animation>::has(name)) {
        result.message = "animation already exists: " + name;
        result.bSuccess = false;
        return result;
    }

    Animation* pAnimation = Loader::fbxAnimation(path);

    bool success = AssetManager<Animation>::add(name, pAnimation);
    assert(success);

    result.message = "Animation load success";
    result.bSuccess = true;
    return result;
}
*/


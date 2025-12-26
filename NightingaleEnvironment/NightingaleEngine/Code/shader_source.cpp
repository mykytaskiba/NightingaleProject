#include "pch.h"
#include "shader_source.h"
#include "defines.h"

ShaderSource ShaderSource::fromFile(string const& filename)
{
    string code;

    ifstream shaderFile;
    stringstream shaderBuffer;


    shaderFile.open(filename);

    if (!shaderFile.is_open()) {
        //_logger.log("Could not open file");
        throw exception("Could not read shader");
    }

    ShaderSource result;

    shaderBuffer << shaderFile.rdbuf();
    result.m_source = shaderBuffer.str();

    return result;
}

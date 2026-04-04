#include "pch.h"
#include "shader_source.h"
#include <fstream>
#include <exception>

ShaderSource ShaderSource::fromFile(std::string const& filename)
{
    std::string code;

    std::ifstream shaderFile;
    std::stringstream shaderBuffer;


    shaderFile.open(filename);

    if (!shaderFile.is_open()) {
        //_logger.log("Could not open file");
        throw std::exception("Could not read shader");
    }

    ShaderSource result;

    shaderBuffer << shaderFile.rdbuf();
    result.m_source = shaderBuffer.str();

    return result;
}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

uniform mat4 uModelMatrix;

uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

out vec3 ioNorm;

void main() {

ioNorm = (uModelMatrix*vec4(aNorm, 0.0)).xyz;
ioNorm = normalize(ioNorm);

gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * vec4(aPos.x, aPos.y, aPos.z,1.0);


}
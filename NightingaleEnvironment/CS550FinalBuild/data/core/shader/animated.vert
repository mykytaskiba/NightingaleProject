#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;
layout (location = 3) in uvec4 aBoneIdx;
layout (location = 4) in vec4 aBoneWeights;

uniform mat4 uModelMatrix;

uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

uniform mat4 uBoneMatrix[100];

out vec3 ioNorm;

void main() {


mat4 finalTransform = mat4(0.0);
    finalTransform += uBoneMatrix[aBoneIdx.x] * aBoneWeights.x;
    finalTransform += uBoneMatrix[aBoneIdx.y] * aBoneWeights.y;
    finalTransform += uBoneMatrix[aBoneIdx.z] * aBoneWeights.z;
    finalTransform += uBoneMatrix[aBoneIdx.w] * aBoneWeights.w;

ioNorm = (finalTransform*vec4(aNorm, 0.0)).xyz;
ioNorm = normalize(ioNorm);

gl_Position = uProjMatrix * uViewMatrix * finalTransform * vec4(aPos.x, aPos.y, aPos.z,1.0);


}
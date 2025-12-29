#version 330 core
layout (location = 0) in float aLinePt; //line point interpolant //either 1.0 or 0.0

uniform mat4 uViewMatrix;
uniform mat4 uProjMatrix;

uniform vec3 uFromPoint;
uniform vec3 uDestPoint;


void main() {

vec3 pos = mix(uFromPoint, uDestPoint, aLinePt);

gl_Position = uProjMatrix * uViewMatrix * vec4(pos,1.0);


}
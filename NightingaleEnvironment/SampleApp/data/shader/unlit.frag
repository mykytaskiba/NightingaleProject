#version 330 core

out vec4 FragColor;

uniform vec4 uColor;

in vec3 ioNorm;

void main()
{ 

FragColor = vec4(uColor.rgb, 1.0f); 

}
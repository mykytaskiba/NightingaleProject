#version 330 core

out vec4 FragColor;

uniform vec4 uColor;

in vec3 ioNorm;

void main()
{ 

float l = max(dot(ioNorm, vec3(0.7f,0.5f,0.3f)), 0.0f);

vec3 ambient_light = vec3(0.06,0.06,0.12);
vec3 color = l * uColor.rgb + ambient_light;
FragColor = vec4(color, 1.0f); 

}
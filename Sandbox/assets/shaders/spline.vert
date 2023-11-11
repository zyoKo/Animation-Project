#version 460 core

layout(location = 0) in vec3 spline;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(spline, 1.0);
}
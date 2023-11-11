#version 460 core

layout(location = 0) in vec3 controlPoint;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_PointSize = 8.0f;
    gl_Position = projection * view * vec4(controlPoint, 1.0);
}
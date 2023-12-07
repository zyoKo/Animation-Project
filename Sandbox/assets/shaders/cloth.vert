#version 460 core

layout(location = 0) in vec3 pointPosition;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_PointSize = 5.0f;
    gl_Position = projection * view * vec4(pointPosition, 1.0);
}
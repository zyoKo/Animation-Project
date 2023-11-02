#version 460 core

uniform mat4 projection;
uniform mat4 view;
uniform vec3 pointPosition;

void main()
{
    gl_PointSize = 20.0f;
    gl_Position = projection * view * vec4(pointPosition, 1.0);
}
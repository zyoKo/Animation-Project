#version 460 core

layout(location = 0) in vec3 bonePos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * vec4(bonePos, 1.0);

}
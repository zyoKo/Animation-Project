#version 460 core

layout(location = 0) in vec3 bonePos;
//layout(location = 1) in vec3 bonesColor;

uniform mat4 projection;
uniform mat4 view;

//out vec4 boneColor;

void main()
{
    //boneColor = projection * view * vec4(bonesColor, 1.0);

    gl_Position = projection * view * vec4(bonePos, 1.0);

}
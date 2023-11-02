#version 460 core

in vec2 TexCoords;

uniform sampler2D gridTexture;

out vec4 FragColor;

void main()
{
    FragColor =  texture(gridTexture, TexCoords);
}
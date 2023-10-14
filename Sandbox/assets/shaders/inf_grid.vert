#version 460 core

layout(location = 0) in vec3 gridColor;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec3 nearPoint;
layout(location = 2) out vec3 farPoint;

uniform mat4 projection;
uniform mat4 view;

vec3 gridPlane[6] = {
    vec3(1, 1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
    vec3(-1, -1, 0), vec3(1, 1, 0), vec3(1, -1, 0)
};

vec3 UnprojectedPlane(float x, float y, float z, mat4 view, mat4 projection)
{
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint = viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main()
{
    gl_Position = projection * view * vec4(gridPlane[gl_VertexID].xyz, 1.0);

    outColor = vec4(gridColor, 1.0);
}
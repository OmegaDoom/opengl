#version 430 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out VS_OUT
{
    vec3 color;
} vs_out;

void main()
{
    gl_Position = vec4(aPos, 0.f, 1.f);
    vs_out.color = aColor;
}

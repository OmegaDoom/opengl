#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_OUT
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
} gs_in[];

out GS_OUT
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
} gs_out;

void main()
{
    gl_Position = gl_in[0].gl_Position;
    gs_out.vColor = gs_in[0].vColor;
    gs_out.texCoords = gs_in[0].texCoords;
    gs_out.normalCoords = gs_in[0].normalCoords;
    gs_out.fragPos = gs_in[0].fragPos;
    EmitVertex();

    gl_Position = gl_in[1].gl_Position;
    gs_out.vColor = gs_in[1].vColor;
    gs_out.texCoords = gs_in[1].texCoords;
    gs_out.normalCoords = gs_in[1].normalCoords;
    gs_out.fragPos = gs_in[1].fragPos;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    gs_out.vColor = gs_in[2].vColor;
    gs_out.texCoords = gs_in[2].texCoords;
    gs_out.normalCoords = gs_in[2].normalCoords;
    gs_out.fragPos = gs_in[2].fragPos;
    EmitVertex();

    EndPrimitive();
}

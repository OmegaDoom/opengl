#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in VS_OUT
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
} gs_in[];

const float MAGNITUDE = 0.2f;

void generateLine(int index)
{
    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normalCoords, 0.f) * MAGNITUDE;
    EmitVertex();
    EndPrimitive();
}

void main()
{
    generateLine(0);
    generateLine(1);
    generateLine(2);
}

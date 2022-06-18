#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

const float size = 0.2f;

in VS_OUT
{
    vec3 color;
} gs_in[];

out vec3 fColor;

void buildGeometry(vec4 pos)
{
    gl_Position = vec4(pos.x - size, pos.y - size, 0.f, 1.f);
    fColor = gs_in[0].color;
    EmitVertex();

    gl_Position = vec4(pos.x + size, pos.y - size, 0.f, 1.f);
    EmitVertex();

    gl_Position = vec4(pos.x - size, pos.y + size, 0.f, 1.f);
    EmitVertex();

    gl_Position = vec4(pos.x + size, pos.y + size, 0.f, 1.f);
    EmitVertex();

    gl_Position = vec4(pos.x, pos.y + 1.5f * size, 0.f, 1.f);
    fColor = vec3(1.f, 1.f, 1.f);
    EmitVertex();

    EndPrimitive();
}

void main()
{    
    buildGeometry(gl_in[0].gl_Position); 
}  

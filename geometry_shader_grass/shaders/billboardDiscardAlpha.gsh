#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 view;
    uniform mat4 camera;
    uniform mat4 projection;
};

out DATA_OUT
{
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
} gs_out;

const float size = 0.5f;

void CreateVertex(vec4 localPos, vec2 texCoords)
{
    vec4 pos = gl_in[0].gl_Position;
    pos.y += size;
    mat4 billboardTransform = mat4(camera[0], camera[1], camera[2], pos);
    gl_Position = projection * view * billboardTransform * localPos; 
    gs_out.texCoords = texCoords;
    gs_out.normalCoords = mat3(view * billboardTransform) * vec3(0.f, 0.f, -1.f);
    gs_out.fragPos = vec3(view * billboardTransform * localPos);
    EmitVertex();
}

void main()
{
    CreateVertex(vec4(-size, size, 0.f, 1.0f), vec2(0.f, 1.f));
    CreateVertex(vec4(-size, -size, 0.f, 1.0f), vec2(0.f, 0.f));
    CreateVertex(vec4(size, size, 0.f, 1.0f), vec2(1.f, 1.f));
    CreateVertex(vec4(size, -size, 0.f, 1.0f), vec2(1.f, 0.f));
    EndPrimitive();
}

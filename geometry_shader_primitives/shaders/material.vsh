#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;

out VS_OUT
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
} vs_out;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 view;
    uniform mat4 camera;
    uniform mat4 projection;
};

uniform mat4 model;
uniform mat4 normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.xyz, 1.0f);
    vs_out.vColor = aColor;
    vs_out.texCoords = aTexCoords;
    vs_out.normalCoords = mat3(normal) * aNormalCoords;
    vs_out.fragPos = vec3(view * model * vec4(aPos.xyz, 1.0f));
}

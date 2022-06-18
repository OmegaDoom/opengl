#version 430 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 view;
    uniform mat4 projection;
};

uniform mat4 model;
uniform mat4 normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.xyz, 1.0f);
}

#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 view;
    uniform mat4 projection;
};

uniform mat4 model;
uniform mat4 normal;

out vec3 texCoords;

void main()
{
    mat4 rot_view = mat4(mat3(view));
    vec4 pos = projection * rot_view * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    gl_Position = pos.xyww;
    texCoords = aPos;
}

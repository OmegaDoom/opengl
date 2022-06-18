#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;
layout (location = 4) in mat4 aInstanceTransform;

out vec3 vColor;
out vec2 texCoords;
out vec3 normalCoords;
out vec3 fragPos;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 view;
    uniform mat4 projection;
};

uniform mat4 model;
uniform mat4 normal;

void main()
{
    gl_Position = projection * view * aInstanceTransform * vec4(aPos.xyz, 1.0f);
    vColor = aColor; 
    texCoords = aTexCoords;
    normalCoords = mat3(normal) * aNormalCoords;
    fragPos = vec3(view * aInstanceTransform * vec4(aPos.xyz, 1.0f));
}

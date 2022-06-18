#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;

out vec3 vColor;
out vec2 texCoords;
out vec3 normalCoords;
out mat4 cameraTransform;
out vec3 fragPos;

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
    vColor = aColor;
    texCoords = aTexCoords;
    normalCoords = mat3(normal) * aNormalCoords;
    cameraTransform = camera;
    fragPos = vec3(view * model * vec4(aPos.xyz, 1.0f));
}

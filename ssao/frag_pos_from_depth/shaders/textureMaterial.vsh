#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;

out vec3 vColor;
out vec2 texCoords;
out vec3 normalCoords;
out vec3 fragPos;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 viewTransform;
    uniform mat4 projectionTransform;
};

uniform mat4 modelTransform;
uniform mat4 normalTransform;

void main()
{
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(aPos.xyz, 1.0f);
    vColor = aColor; 
    texCoords = aTexCoords;
    normalCoords = mat3(normalTransform) * aNormalCoords;
    fragPos = vec3(viewTransform * modelTransform * vec4(aPos.xyz, 1.0f));
}

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
    uniform mat4 inverseViewRotation;
    uniform mat4 projection;
};

uniform mat4 model;
uniform mat4 normal;

void main()
{
    mat4 rotationTransform = mat4(aInstanceTransform[0], aInstanceTransform[1], aInstanceTransform[2], vec4(0, 0, 0, 1));
    mat4 translationTransform = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), aInstanceTransform[3]);
    //mat4 inverseViewRotation = transpose(mat4(view[0], view[1], view[2], vec4(0, 0, 0, 1)));
    mat4 instanceTransform = translationTransform * inverseViewRotation * rotationTransform;
    gl_Position = projection * view * instanceTransform * vec4(aPos.xyz, 1.0f);
    vColor = aColor; 
    texCoords = aTexCoords;
    normalCoords = mat3(view * instanceTransform) * aNormalCoords;
    fragPos = vec3(view * instanceTransform * vec4(aPos.xyz, 1.0f));
}

#version 330 core
#extension GL_ARB_shading_language_include : require
#include "/Constants"
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;

out vec3 vColor;
out vec2 texCoords;
out vec3 normalCoords;
out vec3 fragPos;

uniform mat4 MODEL_TRANSFORM;
uniform mat4 VIEW_TRANSFORM;
uniform mat4 PROJECTION_TRANSFORM;
uniform mat4 NORMAL_TRANSFORM;

void main()
{
    gl_Position = PROJECTION_TRANSFORM * VIEW_TRANSFORM * MODEL_TRANSFORM * vec4(aPos.xyz, 1.0f);
    vColor = aColor; 
    texCoords = aTexCoords;
    normalCoords = mat3(NORMAL_TRANSFORM) * aNormalCoords;
    fragPos = vec3(VIEW_TRANSFORM * MODEL_TRANSFORM * vec4(aPos.xyz, 1.0f));
}

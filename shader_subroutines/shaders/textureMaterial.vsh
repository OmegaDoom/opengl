#version 330 core
#extension GL_ARB_shading_language_include : require
#include "/Constants"
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;

out vec3 vColor;
out vec2 texCoords;
out vec3 normalCoords;
out vec3 fragPos;
out mat3 tbn;

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

    mat3 normalTransform = mat3(NORMAL_TRANSFORM);
    vec3 t = normalize(normalTransform * aTangent);
    vec3 n = normalize(normalTransform * aNormalCoords);
    vec3 b = normalize(normalTransform * aBitangent);
    // re-orthogonalize T with respect to N
    t = normalize(t - dot(t, n) * n);
    //vec3 b = normalize(cross(n, t));
    // then retrieve perpendicular vector B with the cross product of T and N
    tbn = mat3(t, b, n);
}

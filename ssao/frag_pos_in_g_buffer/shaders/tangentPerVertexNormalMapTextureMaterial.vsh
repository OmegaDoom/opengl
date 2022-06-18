#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;


out VertexData
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
    mat3 tbn;
} outData;

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
    outData.vColor = aColor;
    outData.texCoords = aTexCoords;
    mat3 normTransform = mat3(normalTransform);
    outData.normalCoords = normTransform * aNormalCoords;
    outData.fragPos = vec3(viewTransform * modelTransform * vec4(aPos.xyz, 1.0f));

    vec3 t = normalize(normTransform * aTangent);
    vec3 n = normalize(normTransform * aNormalCoords);
    //vec3 b = normalize(normTransform * aBitangent);
    // re-orthogonalize T with respect to N
    t = normalize(t - dot(t, n) * n);
    vec3 b = normalize(cross(n, t));
    // then retrieve perpendicular vector B with the cross product of T and N
    outData.tbn = mat3(t, b, n);
}

#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;

out vec3 vColor;
out vec2 texCoords;
out vec3 normalCoords;
out vec3 fragPos;
out mat3 tbn;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 viewTransform;
    uniform mat4 projectionTransform;
};

uniform mat4 modelTransform;
uniform mat4 normalTransform;
uniform vec3 tangent;

void main()
{
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(aPos.xyz, 1.0f);
    vColor = aColor; 
    texCoords = aTexCoords;
    mat3 normalTransform3 = mat3(normalTransform); 
    normalCoords = normalTransform3 * aNormalCoords;
    fragPos = vec3(viewTransform * modelTransform * vec4(aPos.xyz, 1.0f));

    //vec3 t = normalize(normalTransform3 * tangent);
    //vec3 n = normalCoords;

    vec3 t = normalize(vec3(modelTransform * vec4(tangent, 0.0f)));
    vec3 n = normalize(vec3(modelTransform * vec4(aNormalCoords, 0.0f)));

    // re-orthogonalize T with respect to N
    t = normalize(t - dot(t, n) * n);
    vec3 b = normalize(cross(t, n));
    // then retrieve perpendicular vector B with the cross product of T and N
    tbn = mat3(t, b, n);
}

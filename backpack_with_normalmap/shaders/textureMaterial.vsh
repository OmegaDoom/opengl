#version 330 core
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

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal;

void main()
{
    gl_Position = projection * view * model * vec4(aPos.xyz, 1.0f);
    vColor = aColor; 
    texCoords = aTexCoords;
    normalCoords = mat3(normal) * aNormalCoords;
    fragPos = vec3(view * model * vec4(aPos.xyz, 1.0f));

    mat3 normalTransform = mat3(normal);
    vec3 t = normalize(normalTransform * aTangent);
    vec3 n = normalize(normalTransform * aNormalCoords);
    //vec3 b = normalize(normalTransform * aBitangent);
    // re-orthogonalize T with respect to N
    t = normalize(t - dot(t, n) * n);
    vec3 b = normalize(cross(n, t));
    // then retrieve perpendicular vector B with the cross product of T and N
    tbn = mat3(t, b, n);
}

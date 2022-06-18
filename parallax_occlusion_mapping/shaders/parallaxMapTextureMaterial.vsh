#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;

out VS_OUT
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
    vec3 tangentFragPos;
    vec3 tangentViewPos;
    mat3 tbn;
} vs_out;

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
    vs_out.vColor = aColor; 
    vs_out.texCoords = aTexCoords;
    mat3 normalTransform3 = mat3(normalTransform); 
    vs_out.normalCoords = normalTransform3 * aNormalCoords;
    vs_out.fragPos = vec3(viewTransform * modelTransform * vec4(aPos.xyz, 1.0f));

    vec3 t = normalize(normalTransform3 * tangent);
    vec3 n = normalize(vs_out.normalCoords);

    // re-orthogonalize T with respect to N
    //t = normalize(t - dot(t, n) * n);
    vec3 b = normalize(cross(t, n));
    // then retrieve perpendicular vector B with the cross product of T and N
    vs_out.tbn = mat3(t, b, n);
    mat3 invTbn = transpose(vs_out.tbn);
    vs_out.tangentFragPos = invTbn * vs_out.fragPos; 
    vs_out.tangentViewPos = invTbn * vec3(0, 0, 0); 
}

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormalCoords;
layout (location = 4) in ivec4 aBoneIDs;
layout (location = 5) in vec4 aWeights;

const int MAX_BONES = 100;

out vec3 vColor;
out vec2 texCoords;
out vec3 normalCoords;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal;
uniform mat4 bones[MAX_BONES];

void main()
{
    mat4 boneTransform = bones[aBoneIDs[0]] * aWeights[0];

    for (int i = 0; i < 3; ++i)
    {
        boneTransform += bones[aBoneIDs[i + 1]] * aWeights[i + 1];
    }

    vec4 pos = boneTransform * vec4(aPos, 1.f);
    vec4 norm = boneTransform * vec4(aNormalCoords, 0);
    gl_Position = projection * view * model * pos; 
    vColor = aColor; 
    texCoords = aTexCoords;
    normalCoords = mat3(normal) * normalize(norm.xyz);
    fragPos = vec3(view * model * pos);
}

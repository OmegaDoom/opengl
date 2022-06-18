#version 430 core
in vec3 vColor;
in vec2 texCoords;
in vec3 normalCoords;
in vec3 fragPos;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormalShine;
layout (location = 2) out vec4 gAlbedoSpec;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main()
{
    gPosition = fragPos;
    gNormalShine.xyz = normalCoords;
    gNormalShine.w = material.shininess;
    gAlbedoSpec.rgb = material.diffuse; 
    gAlbedoSpec.a = material.specular.r; 
}

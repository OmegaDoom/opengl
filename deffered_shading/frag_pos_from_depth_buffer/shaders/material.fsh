#version 430 core
in vec3 vColor;
in vec2 texCoords;
in vec3 normalCoords;
in vec3 fragPos;

layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec4 gNormalShine;

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
    gAlbedoSpec.rgb = material.diffuse; 
    gAlbedoSpec.w = float(material.specular); 
    gNormalShine.xyz = normalCoords;
    gNormalShine.w = material.shininess;
}

#version 430 core
in vec3 vColor;
in vec2 texCoords;
in vec3 normalCoords;
in vec3 fragPos;

layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec4 gNormalShine;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

void main()
{
    gAlbedoSpec.rgb = texture(material.diffuse, texCoords).rgb;
    gAlbedoSpec.w = texture(material.specular, texCoords).r;
    gNormalShine.xyz = normalCoords;
    gNormalShine.w = material.shininess;
}

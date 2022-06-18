#version 430 core
in VertexData
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
    mat3 tbn;
} inData;

layout (location = 0) out vec4 gAlbedoSpec;
layout (location = 1) out vec4 gNormalShine;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    sampler2D normalmap;
};

uniform Material material;

void main()
{
    vec3 normal = texture(material.normalmap, inData.texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(inData.tbn * normal);

    gAlbedoSpec.rgb = texture(material.diffuse, inData.texCoords).rgb;
    gAlbedoSpec.w = texture(material.specular, inData.texCoords).r;
    gNormalShine.xyz = normal;
    gNormalShine.w = material.shininess;
}

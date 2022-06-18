#version 430 core
in vec2 texCoords;
out vec4 fragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

void main()
{
    vec2 texCoords0 = texCoords;
    vec2 tex_offset = 1.0 / textureSize(material.diffuse, 0); // gets size of single texel
    fragColor = texture(material.diffuse, texCoords + vec2(tex_offset.x * 0.5, tex_offset.y * 0.5));
    //fragColor = texture(material.diffuse, texCoords0);
}

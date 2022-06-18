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
    vec3 color = texture(material.diffuse, texCoords0).rgb;
    vec2 tex_offset = 1.0 / textureSize(material.diffuse, 0); // gets size of single texel
    vec3 result = color.rgb; // current fragment's contribution
    result += texture(material.diffuse, vec2(texCoords0.x + tex_offset.x, texCoords0.y)).rgb;
    result += texture(material.diffuse, vec2(texCoords0.x, texCoords0.y + tex_offset.y)).rgb;
    result += texture(material.diffuse, vec2(texCoords0.x + tex_offset.x, texCoords0.y + tex_offset.y)).rgb;
    fragColor = vec4(result / 4, 1.0);
}

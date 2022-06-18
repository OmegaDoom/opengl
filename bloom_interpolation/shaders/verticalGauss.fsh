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
uniform float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
uniform float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

void main()
{
    vec3 color = texture(material.diffuse, texCoords).rgb;
    vec2 tex_offset = 1.0 / textureSize(material.diffuse, 0); // gets size of single texel
    vec3 result = color.rgb * weight[0]; // current fragment's contribution
    for(int i = 1; i < 3; ++i)
    {
        result += texture(material.diffuse, texCoords + vec2(0.0, offset[i] * tex_offset.y)).rgb * weight[i];
        result += texture(material.diffuse, texCoords - vec2(0.0, offset[i] * tex_offset.y)).rgb * weight[i];
    }
    fragColor = vec4(result, 1.0);
}

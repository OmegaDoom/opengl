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
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec3 color = texture(material.diffuse, texCoords).rgb;
    vec2 tex_offset = 1.0 / textureSize(material.diffuse, 0); // gets size of single texel
    vec3 result = color.rgb * weight[0]; // current fragment's contribution
    for(int i = 1; i < 5; ++i)
    {
        result += texture(material.diffuse, texCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        result += texture(material.diffuse, texCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
    }
    fragColor = vec4(result, 1.0);
}

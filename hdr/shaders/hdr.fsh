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
    // apply gamma correction
    float gamma = 2.2;
    vec3 hdrColor = texture(material.diffuse, texCoords).rgb;
    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    //fragColor = vec4(hdrColor, 1.0);
    //fragColor = vec4(pow(FragColor.rgb, vec3(1.0/gamma)), 1.0);
    fragColor = vec4(mapped, 1.0);
}

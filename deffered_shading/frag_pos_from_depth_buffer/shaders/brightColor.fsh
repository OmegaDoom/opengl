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
    vec3 color = texture(material.diffuse, texCoords).rgb;
    float brightness = dot(color.rgb, vec3(0.5122, 0.5152, 0.5152));
    if(brightness > 1.0)
        fragColor = vec4(color.rgb, 1.0);
    else
        fragColor = vec4(0.0, 0.0, 0.0, 1.0);
}

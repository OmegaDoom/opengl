#version 430 core
in vec2 texCoords;
out vec4 fragColor;

#define MAX_TEXTURES 8

struct Textures
{
    int count;
    sampler2D[MAX_TEXTURES] textures;
};

uniform Textures textures;

void main()
{
    vec3 hdrColor = vec3(0.0); 

    for (int i = 0; i < textures.count; ++i)
    {
        hdrColor += texture(textures.textures[i], texCoords).rgb;
    }
    // apply gamma correction
    float gamma = 2.2;
    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0, 1.0, 1.0));
    //fragColor = vec4(hdrColor, 1.0);
    //fragColor = vec4(pow(FragColor.rgb, vec3(1.0/gamma)), 1.0);
    fragColor = vec4(mapped, 1.0);
}

#version 430 core
in vec2 texCoords;
out float fragColor;

#define MAX_TEXTURES 8

struct Textures
{
    int count;
    sampler2D[MAX_TEXTURES] textures;
};

uniform Textures textures;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 viewTransform;
    uniform mat4 projectionTransform;
    uniform mat4 inversedProjectionTransform;
};

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(textures.textures[0], 0));
    float result = 0.0;
    for (int x = -2; x < 2; ++x)
    {
        for (int y = -2; y < 2; ++y)
        {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(textures.textures[0], texCoords + offset).r;
        }
    }
    fragColor = result / (4.0 * 4.0);
}
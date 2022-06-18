#version 430 core
in VS_OUT
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
    vec3 tangentFragPos;
    vec3 tangentViewPos;
    mat3 tbn;
} fs_in;

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormalShine;
layout (location = 2) out vec4 gAlbedoSpec;

#define MAX_POINT_LIGHTS 8

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normalMapping;
    sampler2D disp;
    float shininess;
};

uniform Material material;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 viewTransform;
    uniform mat4 projectionTransform;
};

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
    vec3 tangentViewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec2 texCoords = parallaxMapping(fs_in.texCoords, tangentViewDir);

    //if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
    //    discard;

    // obtain normalCoords from normalCoords map in range [0,1]
    vec3 normal = texture(material.normalMapping, texCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fs_in.tbn * normal);
    //normal = fs_in.normalCoords;

    gPosition = fs_in.fragPos;
    gNormalShine.xyz = normal;
    gNormalShine.w = material.shininess;
    gAlbedoSpec.rgb = texture(material.diffuse, texCoords).rgb;
    gAlbedoSpec.a = texture(material.specular, texCoords).r;
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir)
{
    const float height_scale  = 0.1f;
    const float minLayers = 8.0;
    const float maxLayers = 24.0;
    const float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    const float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 p = viewDir.xy * height_scale;
    vec2 deltaTexCoords = p / numLayers;
    vec2 currentTexCoords = texCoords;
    float height = 1.0 - texture(material.disp, currentTexCoords).r;

    while(currentLayerDepth < height)
    {
        currentTexCoords -= deltaTexCoords;
        height = 1.0 - texture(material.disp, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    // get depth after and before collision for linear interpolation
    float afterDepth = currentLayerDepth - height;
    float beforeDepth = 1.0 - texture(material.disp, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth + beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

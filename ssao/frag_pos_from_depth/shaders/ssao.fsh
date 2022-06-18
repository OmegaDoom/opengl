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

uniform sampler2D texNoise;
uniform vec3 samples[64];
uniform int width;
uniform int height;

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 viewTransform;
    uniform mat4 projectionTransform;
    uniform mat4 inversedProjectionTransform;
};

vec3 reconstructPosition(float z, vec2 texCoord)
{
    float x = texCoord.x * 2.0 - 1;
    float y = texCoord.y * 2.0 - 1;
    vec4 projectedPos = vec4(x, y, z, 1.0f);
    
    vec4 position = inversedProjectionTransform * projectedPos;
    
    return position.xyz / position.w;
}

const float bias = 0.025f;
const int kernelSize = 64;
const float radius = 0.5f;

void main()
{
    // tile noise texture over screen, based on screen dimensions / noise size
    const vec2 noiseScale = vec2(width/4.0, height/4.0);

    vec4 albedoSpec = texture(textures.textures[0], texCoords);
    vec4 normalShine = texture(textures.textures[1], texCoords);
    vec3 normal = normalShine.xyz;
    float depth = 2.0 * texture(textures.textures[2], texCoords).r - 1.0;
    vec3 fragPos = reconstructPosition(depth, texCoords); 
    vec3 albedo = albedoSpec.rgb;
    vec3 randomVec = texture(texNoise, texCoords * noiseScale).xyz;
    vec3 tangent = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN = mat3(tangent, bitangent, normal);

    float occlusion = 0.f;
    for(int i = 0; i < kernelSize; ++i)
    {
        // get sample position
        vec3 smpl = TBN * samples[i]; // from tangent to view-space
        smpl = fragPos + smpl * radius;

        vec4 offset = vec4(smpl, 1.0);
        offset = projectionTransform * offset;
        // from view to clip-space
        offset.xyz /= offset.w;
        // perspective divide
        offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0
        float sampleDepth = 2.f * texture(textures.textures[2], offset.xy).r - 1.f;
        vec3 samplePoint = reconstructPosition(sampleDepth, offset.xy);
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.z - sampleDepth));
        occlusion += (samplePoint.z >= smpl.z + bias ? 1.0 : 0.0);
    }
    occlusion = 1.f - (occlusion / kernelSize);
    fragColor = vec4(occlusion, occlusion, occlusion, 1.f);
}
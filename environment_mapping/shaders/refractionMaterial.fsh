#version 430 core
in vec3 vColor;
in vec2 texCoords;
in vec3 normalCoords;
in vec3 fragPos;
in mat4 cameraTransform;

out vec4 fragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;
uniform samplerCube environment;

const float refraction_ratio = 1.f / 1.52f;

void main()
{
    vec3 viewDir = normalize(fragPos);

    //refraction
    vec3 refractViewDir = normalize(refract(viewDir, normalize(normalCoords), refraction_ratio));
    vec3 worldRefraction = vec3(cameraTransform * vec4(refractViewDir, 0.f));
    vec3 refraction = vec3(texture(environment, worldRefraction));

    fragColor = vec4(refraction, 1.0f);
}

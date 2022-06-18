#version 430 core
in vec3 vColor;
in vec2 texCoords;
in vec3 normalCoords;
flat in mat4 cameraTransform;

in vec3 fragPos;

out vec4 fragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;
uniform samplerCube environment;

void main()
{
    vec3 viewDir = normalize(fragPos);

    //reflection
    vec3 reflectViewDir = normalize(reflect(viewDir, normalCoords));
    vec3 worldReflection = vec3(cameraTransform * vec4(reflectViewDir, 0.f));
    vec3 reflection = vec3(texture(environment, worldReflection));

    fragColor = vec4(reflection, 1.0f);
}

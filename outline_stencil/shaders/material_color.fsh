#version 330 core
in vec3 vColor;
in vec2 texCoords;
in vec3 normalCoords;
in vec3 fragPos;
out vec4 fragColor;

#define MAX_POINT_LIGHTS 8

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct SpotLight
{
    vec3 position; //in view space
    vec3 direction;
    float cutOff;
    float outerCutOff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position; //in view space
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight spotLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight directionalLight;
uniform int pointLightCount;

void main()
{
    fragColor = vec4(material.diffuse.xyz, 1.0f);
}

#version 430 core
in vec3 vColor;
in vec2 texCoords;
in vec3 normalCoords;
in vec3 fragPos;
out vec4 fragColor;

#define MAX_POINT_LIGHTS 8

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct SpotLight
{
    vec3 position; //in view space
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
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

layout (std140, binding = 1) uniform lighting
{
    uniform SpotLight spotLight;
    uniform DirectionalLight directionalLight;
    uniform int pointLightCount;
    uniform PointLight pointLights[MAX_POINT_LIGHTS];
};

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos);

void main()
{
    vec3 color = calcDirectionalLight(directionalLight, normalCoords, fragPos); 

    for(int i = 0; i < pointLightCount; i++)
        color += calcPointLight(pointLights[i], normalCoords, fragPos);

    color += calcSpotLight(spotLight, normalCoords, fragPos); 
    fragColor = vec4(color.xyz, 1.0f);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos)
{
    vec3 lightDir = normalize(-fragPos);        
    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.f / (light.constant + light.linear * distance
                         + light.quadratic * (distance * distance));

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    //duffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.f), 2.f * material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos)
{
    vec3 lightDir = normalize(-light.direction);        

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    //duffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.f), 2.f * material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.f / (light.constant + light.linear * distance
                         + light.quadratic * (distance * distance));

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    //duffuse
    vec3 lightDir = normalize(light.position - fragPos);        
    vec3 norm = normalize(normalCoords);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.f), 2.f * material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

#version 330 core
in GS_OUT
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
} fs_in;

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

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos);

void main()
{
    vec3 color = calcDirectionalLight(directionalLight, fs_in.normalCoords, fs_in.fragPos); 

    for(int i = 0; i < pointLightCount; i++)
        color += calcPointLight(pointLights[i], fs_in.normalCoords, fs_in.fragPos);

    color += calcSpotLight(spotLight, fs_in.normalCoords, fs_in.fragPos); 
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
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.texCoords));

    //duffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.texCoords));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, fs_in.texCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos)
{
    vec3 lightDir = normalize(-light.direction);        

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.texCoords));

    //duffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.texCoords));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, fs_in.texCoords));

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.f / (light.constant + light.linear * distance
                         + light.quadratic * (distance * distance));

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.texCoords));

    //duffuse
    vec3 lightDir = normalize(light.position - fragPos);        
    vec3 norm = normalize(fs_in.normalCoords);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.texCoords));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, fs_in.texCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

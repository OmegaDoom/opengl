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

#define MAX_POINT_LIGHTS 8

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

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, float ambientOcclusion);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, float ambientOcclusion);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, float ambientOcclusion);

Material material;

void main()
{
    vec3 fragPos = texture(textures.textures[0], texCoords).rgb;
    vec4 normalShine = texture(textures.textures[1], texCoords);
    vec3 normalCoords = normalShine.rgb;
    vec4 albedoSpec = texture(textures.textures[2], texCoords);
    vec3 albedo = albedoSpec.rgb;
    vec3 specular = vec3(albedoSpec.a);
    float ambientOcclusion = texture(textures.textures[2], texCoords).r;
    material.ambient = vec3(0);
    material.diffuse = albedo;
    material.specular = specular;
    material.shininess = normalShine.w;

    vec3 color = calcDirectionalLight(directionalLight, normalCoords, fragPos, ambientOcclusion);

    for(int i = 0; i < pointLightCount; i++)
        color += calcPointLight(pointLights[i], normalCoords, fragPos, ambientOcclusion);

    color += calcSpotLight(spotLight, normalCoords, fragPos, ambientOcclusion);

    vec3 hdrColor = color; 

    // apply gamma correction
    float gamma = 2.2;
    // reinhard tone mapping
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0, 1.0, 1.0));
    //fragColor = vec4(hdrColor, 1.0);
    //fragColor = vec4(pow(FragColor.rgb, vec3(1.0/gamma)), 1.0);
    fragColor = vec4(mapped, 1.0);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, float ambientOcclusion)
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
    vec3 ambient = light.ambient * material.diffuse * ambientOcclusion;

    //duffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, float ambientOcclusion)
{
    vec3 lightDir = normalize(-light.direction);        

    //ambient
    vec3 ambient = light.ambient * material.diffuse * ambientOcclusion;

    //duffuse
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, float ambientOcclusion)
{
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.f / (light.constant + light.linear * distance
                         + light.quadratic * (distance * distance));

    //ambient
    vec3 ambient = light.ambient * material.diffuse * ambientOcclusion;

    //duffuse
    vec3 lightDir = normalize(light.position - fragPos);        
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.f);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
    vec3 specular = spec * light.specular * material.specular;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

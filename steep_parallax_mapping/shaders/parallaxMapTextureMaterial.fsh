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

out vec4 fragColor;

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

layout (std140, binding = 0) uniform scene_camera_transformations
{
    uniform mat4 viewTransform;
    uniform mat4 projectionTransform;
};

layout (std140, binding = 1) uniform lighting
{
    uniform SpotLight spotLight;
    uniform DirectionalLight directionalLight;
    uniform int pointLightCount;
    uniform PointLight pointLights[MAX_POINT_LIGHTS];
};

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPosi, vec2 texCoords);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec2 texCoords);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec2 texCoords);

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
    vec3 tangentViewDir = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec2 texCoords = parallaxMapping(fs_in.texCoords, tangentViewDir);

    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    // obtain normalCoords from normalCoords map in range [0,1]
    vec3 normal = texture(material.normalMapping, texCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);
    normal = normalize(fs_in.tbn * normal);
    //normal = fs_in.normalCoords;

    vec3 color = calcDirectionalLight(directionalLight, normal, fs_in.fragPos, texCoords); 

    for(int i = 0; i < pointLightCount; i++)
    {
        vec3 pointColor = calcPointLight(pointLights[i], normal, fs_in.fragPos, texCoords);
        pointColor.x = clamp(pointColor.x, 0.0, 1.0);
        pointColor.y = clamp(pointColor.y, 0.0, 1.0);
        pointColor.z = clamp(pointColor.z, 0.0, 1.0);
        color += pointColor;
    }

    color += calcSpotLight(spotLight, normal, fs_in.fragPos, texCoords); 
    fragColor = vec4(color.xyz, 1.0f);
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir)
{
    const float height_scale  = 0.1f;
    const float minLayers = 8.0;
    const float maxLayers = 32.0;
    const float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    const float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 p = viewDir.xy * height_scale;
    vec2 deltaTexCoords = p / numLayers;
    vec2 currentTexCoords = texCoords;
    float height = texture(material.disp, currentTexCoords).r;

    while(currentLayerDepth < height)
    {
        currentTexCoords -= deltaTexCoords;
        height = texture(material.disp, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    return currentTexCoords;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec2 texCoords)
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
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 2.0 * material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos, vec2 texCoords)
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

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec2 texCoords)
{
    //attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance
                         + light.quadratic * (distance * distance));

    //ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));

    //duffuse
    vec3 lightDir = normalize(light.position - fragPos);        
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));

    //specular
    vec3 viewDir = normalize(-fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 2.0 * material.shininess);
    vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

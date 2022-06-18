#version 430 core

in VS_OUT
{
    vec3 vColor;
    vec2 texCoords;
    vec3 normalCoords;
    vec3 fragPos;
    vec3 fragPosInWorld;
} fs_in;

out vec4 fragColor;

#define MAX_POINT_LIGHTS 8

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
    samplerCube shadowMap;
};

uniform Material material;
uniform vec3 lightPos;
uniform float far_plane;

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

float ShadowCalculation(vec3 fragPos);
vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 fragPos);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos);

void main()
{
    vec3 color = calcDirectionalLight(directionalLight, fs_in.normalCoords, fs_in.fragPos); 

    for(int i = 0; i < pointLightCount; i++)
        color += calcPointLight(pointLights[i], fs_in.normalCoords, fs_in.fragPos);

    color += calcSpotLight(spotLight, fs_in.normalCoords, fs_in.fragPos); 
    float shadow = ShadowCalculation(fs_in.fragPosInWorld);
    fragColor = (1.f - shadow) * vec4(color.xyz, 1.0f);
}

vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);  

float ShadowCalculation(vec3 fragPos)
{
    // get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // use the light to fragment vector to sample from the depth map    
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias   = 0.15;
    int samples  = 20;
    float viewDistance = length(fs_in.fragPos);
    float diskRadius = (1.0 + (viewDistance / far_plane)) / 25.0;  
//    float diskRadius = 0.05;
    for(int i = 0; i < samples; ++i)
    {
        float closestDepth = texture(material.shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;   // Undo mapping [0;1]
        if(currentDepth - bias > closestDepth)
            shadow += 1.0;
    }
    shadow /= float(samples);

    return shadow;
}  

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos)
{
    vec2 texCoords = fs_in.texCoords;
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
    vec2 texCoords = fs_in.texCoords;

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
    vec2 texCoords = fs_in.texCoords;
    vec3 normalCoords = fs_in.normalCoords;

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

#version 460 core

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;

    sampler2D diffuseMap;
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
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight 
{
    vec3 direction;
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};

in FragmentData
{
    vec3 fNormal;
    vec3 fragmentPosition;
    vec2 textureCoordinates;
} fragmentDataIn;

layout(location = 0) out vec3 fragColor;

uniform Material material;
uniform vec3 cameraPosition;

// Max number of light is the array size

uniform DirectionalLight directionalLights[32];
uniform int directionalLightCount;
uniform PointLight pointLights[32];
uniform int pointLightCount;
uniform SpotLight spotLights[32];
uniform int spotLightCount;

vec3 CalculateLight(vec3 lAmbient, vec3 lDiffuse, vec3 lSpecular, float attenuation, vec3 lightDirection, vec3 normal, vec3 cameraDirection, vec2 textureCoordinates, Material material) {
    vec3 ambient = material.ambient * lAmbient * vec3(texture(material.diffuseMap, textureCoordinates)) * attenuation; // not sure material ambient
    vec3 diffuse = material.diffuse * lDiffuse * max(dot(normal, lightDirection), 0.0) * vec3(texture(material.diffuseMap, textureCoordinates)) * attenuation;

    vec3 reflectionDirection = reflect(-lightDirection, normal);
    vec3 specular = material.specular * pow(max(dot(cameraDirection, reflectionDirection), 0.0), material.shininess) * lSpecular * vec3(texture(material.diffuseMap, textureCoordinates)) * attenuation;

    //vec3 specular = { 0,0,0 };
    //vec3 ambient = { 0,0,0 };

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 normal = normalize(fragmentDataIn.fNormal);
    vec3 cameraDirection = normalize(cameraPosition - fragmentDataIn.fragmentPosition);

    vec3 result = {0,0,0};
    
    for (int i = 0; i < directionalLightCount; i++) {
        vec3 lightDirection;
        lightDirection = normalize(-directionalLights[i].direction);

        result += CalculateLight(directionalLights[i].ambient, directionalLights[i].diffuse, directionalLights[i].specular, 1, lightDirection, normal, cameraDirection, fragmentDataIn.textureCoordinates, material);
    }
    
    for (int i = 0; i < pointLightCount; i++) {

        vec3 lightDirection;
        float attenuation = 1;
        lightDirection = normalize(pointLights[i].position - fragmentDataIn.fragmentPosition);

        float distance = length(pointLights[i].position - fragmentDataIn.fragmentPosition);
        attenuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));

        result += CalculateLight(pointLights[i].ambient, pointLights[i].diffuse, pointLights[i].specular, attenuation, lightDirection, normal, cameraDirection, fragmentDataIn.textureCoordinates, material);
    }

    for (int i = 0; i < spotLightCount; i++) {

        vec3 lightDirection;
        float attenuation = 1;
        lightDirection = normalize(spotLights[i].position - fragmentDataIn.fragmentPosition);

        float distance = length(spotLights[i].position - fragmentDataIn.fragmentPosition);
        attenuation = 1.0 / (spotLights[i].constant + spotLights[i].linear * distance + spotLights[i].quadratic * (distance * distance));

        float theta = dot(lightDirection, normalize(-spotLights[i].direction));
        float epsilon = spotLights[i].cutOff - spotLights[i].outerCutOff;
        float intensity = clamp((theta - spotLights[i].outerCutOff) / epsilon, 0.0, 1.0);

        attenuation *= intensity;

        result += CalculateLight(spotLights[i].ambient, spotLights[i].diffuse, spotLights[i].specular, attenuation, lightDirection, normal, cameraDirection, fragmentDataIn.textureCoordinates, material);
    }
    
    //vec3 result = vec3(texture(material.diffuseMap, textureCoordinates));

    fragColor = result;
}
#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
//material
struct Material
{
    //vec3 ambient;
    //vec3 diffuse;
    sampler2D diffuse;
    //vec3 specular;
    sampler2D specular;
    float shininess;
};
uniform Material material;
//light 
// struct Light{
//     vec3 position;
//     vec3 direction;
//     float cutOff;
//     float outerCutOff;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;

//     float constant;
//     float linear;
//     float quadratic;
// };
// uniform Light light;
//DirLight
struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;
//pointLight
struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLight[NR_POINT_LIGHTS];
//spotLight
struct SpotLight{
    vec3 position;
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
uniform SpotLight spotLight;

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir);
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 FragPos,vec3 viewDir);
vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 FragPos,vec3 viewDir);
void main()
{
    //Diffuse
    vec3 norm=normalize(Normal);
    vec3 viewDir=normalize(viewPos-FragPos);
    //DirectionLight
    vec3 result = CalcDirLight(dirLight,norm,viewDir);
    //SpotLight
    result += CalcSpotLight(spotLight,norm,FragPos,viewDir);
    //PointLight
    for(int i=0;i<NR_POINT_LIGHTS;i++)
        result += CalcPointLight(pointLight[i],norm,FragPos,viewDir);
    
    color =vec4(result,1.0);
}

vec3 CalcDirLight(DirLight light,vec3 normal,vec3 viewDir)
{
    vec3 lightDir=normalize(-light.direction);
    float diff=max(dot(normal,lightDir),0.0);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);

    vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));
    vec3 specular=light.specular* spec*vec3(texture(material.specular,TexCoords));

    return (ambient+diffuse+specular);
}

vec3 CalcPointLight(PointLight light,vec3 normal,vec3 FragPos,vec3 viewDir)
{
    vec3 lightDir=normalize(light.position-FragPos);
    float diff=max(dot(normal,lightDir),0.0);
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    
    //attenuation
    float distance=length(light.position-FragPos);
    float attenuation=1.0f / (light.constant+light.linear*distance+light.quadratic*pow(distance,2));

    vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));
    vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));

    ambient *=attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient+diffuse+specular);
}

vec3 CalcSpotLight(SpotLight light,vec3 normal,vec3 FragPos,vec3 viewDir)
{

    vec3 lightDir=normalize(light.position-FragPos);
    float theta=dot(lightDir,normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff) / epsilon, 0.0, 1.0); //computer light ange

    float diff=max(dot(normal,lightDir),0.0);
    vec3 reflectDir=reflect(-lightDir,normal);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    
    //attenuation衰减
    float distance=length(light.position-FragPos);
    float attenuation=1.0f / (light.constant+light.linear*distance+light.quadratic*pow(distance,2));

    vec3 ambient=light.ambient*vec3(texture(material.diffuse,TexCoords));
    vec3 diffuse=light.diffuse*diff*vec3(texture(material.diffuse,TexCoords));
    vec3 specular=light.specular*spec*vec3(texture(material.specular,TexCoords));

    diffuse *=intensity;
    specular *=intensity;

    return (ambient+diffuse+specular);
}

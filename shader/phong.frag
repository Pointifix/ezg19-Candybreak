#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
	vec3 diffuseColor;
    float shininess;
	bool diffuseMode;
}; 

struct DirectionalLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform mat4 lightView;
uniform mat4 lightProjection;

uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform DirectionalLight directionalLight;
uniform Material material;

vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace, DirectionalLight light, vec3 normal, vec3 lightDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec4 result = CalcDirectionalLight(directionalLight, norm, viewDir);

    FragColor = result;
}

vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec4 color;

	if (material.diffuseMode) color = vec4(material.diffuseColor, 1.0);
	else color = texture(material.diffuse, TexCoords);

    vec4 ambient = vec4(light.ambient, 1.0) * color;
	vec4 diffuse = vec4(light.diffuse, 1.0) * diff * color;
    vec4 specular = vec4(light.specular, 1.0) * spec * color;

	float shadow = ShadowCalculation(FragPosLightSpace, light, normal, lightDir);

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

float ShadowCalculation(vec4 fragPosLightSpace, DirectionalLight light, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

	// if outside of shadow map return no shadow
	if(projCoords.z > 1.0) return 0.0;

    float currentDepth = projCoords.z;

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; x++)
	{
		for(int y = -1; y <= 1; y++)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - 0.001 > pcfDepth ? 1.0 : 0.0;        
		}
	}

    return shadow / 9.0;
}  
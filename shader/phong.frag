#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
	vec4 diffuseColor;
    float shininess;
	bool diffuseMode;
};

struct DirectionalLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in VS_OUT {
	in vec3 FragPos;
	in vec3 Normal;
	in vec2 TexCoords;
	in vec4 FragPosLightSpace;
} fs_in;

uniform bool isALight;

uniform sampler2D shadowMap;

flat in int instanceID;
uniform bool isInstanced;

uniform float t;

uniform vec3 viewPos;
uniform Material material;

#define NR_SPOTLIGHTS 4

uniform DirectionalLight directionalLight;
uniform SpotLight spotLights[NR_SPOTLIGHTS];

uniform bool spotLightsOn;
uniform int level;

vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float ShadowCalculation(vec4 fragPosLightSpace, DirectionalLight light, vec3 normal, vec3 lightDir);

void main()
{
    vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    
	vec4 result;

	if (isALight)
	{
		result = material.diffuseColor;
	}
	else
	{
		result += CalcDirectionalLight(directionalLight, norm, viewDir);

		if (spotLightsOn)
		{
			for(int i = 0; i < NR_SPOTLIGHTS; i++)
			{
				result += CalcSpotLight(spotLights[i], norm, fs_in.FragPos, viewDir);
			}
		}

		if (result.a > 1.0) result.a = 1.0;
	}
    FragColor = result;
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	vec4 color;

	if(isInstanced)
	{
		ivec3 blockIndices;
		blockIndices.x = int((fs_in.FragPos.x - 25) / 5) % 10;
		blockIndices.y = int((fs_in.FragPos.y) / 2.5) % 10;
		blockIndices.z = int((fs_in.FragPos.z - 25) / 5) % 10;

		const vec4 colorarray[7] = {
			vec4(0.0, 0.0, 1.0, 1.0),
			vec4(0.0, 1.0, 0.0, 1.0),
			vec4(0.0, 1.0, 1.0, 1.0),
			vec4(1.0, 0.0, 0.0, 1.0),
			vec4(1.0, 0.0, 1.0, 1.0),
			vec4(1.0, 1.0, 0.0, 1.0),
			vec4(1.0, 0.0, 1.0, 1.0)
		};

		float randomNumber = rand(vec2(int(t * 4000) + blockIndices.x + 10 * blockIndices.y + 100 * blockIndices.z));
		if (t > 0.745)
		{
			const vec4 colorarray[7] = {
				vec4(0.0, 0.0, 20.0, 1.0),
				vec4(0.0, 20.0, 0.0, 1.0),
				vec4(0.0, 20.0, 20.0, 1.0),
				vec4(20.0, 0.0, 0.0, 1.0),
				vec4(20.0, 0.0, 20.0, 1.0),
				vec4(20.0, 1.0, 0.0, 1.0),
				vec4(20.0, 0.0, 20.0, 1.0)
			};
			color = colorarray[int(randomNumber * 7) % 7];
		}
		else if (t > 0.297)
		{
			color = colorarray[((blockIndices.y) + int(t * 4000)) % 7];
		}
		else
		{
			if (level <= 10 - blockIndices.y) discard;
			color = colorarray[(blockIndices.y) % 7];
		}
	}
	else
	{
		if (material.diffuseMode) color = material.diffuseColor;
		else color = texture(material.diffuse, fs_in.TexCoords);
	}

    vec4 ambient = vec4(light.ambient, 1.0) * color;
	vec4 diffuse = vec4(0);
    vec4 specular = vec4(0);
	float shadow = 0.0;
	if (!spotLightsOn)
	{
		diffuse = vec4(light.diffuse, 1.0) * diff * color;
		specular = vec4(light.specular, 1.0) * spec * color;
		shadow = ShadowCalculation(fs_in.FragPosLightSpace, light, normal, lightDir);
	}

    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
	vec4 color;
	if (material.diffuseMode) color = material.diffuseColor;
	else color = texture(material.diffuse, fs_in.TexCoords);

    vec3 ambient = light.ambient * vec3(color);
    vec3 diffuse = light.diffuse * diff * vec3(color);
    vec3 specular = light.specular * spec * vec3(color);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return vec4((ambient + diffuse + specular), 1.0);
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
			float bias = max(0.0003 * (1.0 - dot(normal, lightDir)), 0.00003);  
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}
	}

    return shadow / 9.0;
}  
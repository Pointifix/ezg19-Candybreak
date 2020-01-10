#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
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
uniform DirectionalLight directionalLight;
uniform Material material;

vec4 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir);
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
		result = CalcDirectionalLight(directionalLight, norm, viewDir);
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
		ivec3 blockIndices = ivec3((fs_in.FragPos) / 5.0) % 10;

		float randomNumber = rand(vec2(int(t * 200) + blockIndices.x + 10 * blockIndices.y + 100 * blockIndices.z));

		const vec4 colorarray[14] = {
			vec4(0.0, 0.0, 1.0, 1.0),
			vec4(0.0, 1.0, 0.0, 1.0),
			vec4(0.0, 1.0, 1.0, 1.0),
			vec4(1.0, 0.0, 0.0, 1.0),
			vec4(1.0, 0.0, 1.0, 1.0),
			vec4(1.0, 1.0, 0.0, 1.0),
			vec4(1.0, 0.0, 1.0, 1.0),
			vec4(0.0, 0.0, 50.0, 1.0),
			vec4(0.0, 50.0, 0.0, 1.0),
			vec4(0.0, 50.0, 50.0, 1.0),
			vec4(50.0, 0.0, 0.0, 1.0),
			vec4(50.0, 0.0, 50.0, 1.0),
			vec4(50.0, 1.0, 0.0, 1.0),
			vec4(50.0, 0.0, 50.0, 1.0)
		};
		color = colorarray[int(randomNumber * 14) % 14];
	}
	else
	{
		if (material.diffuseMode) color = material.diffuseColor;
		else color = texture(material.diffuse, fs_in.TexCoords);
	}

    vec4 ambient = vec4(light.ambient, 1.0) * color;
	vec4 diffuse = vec4(light.diffuse, 1.0) * diff * color;
    vec4 specular = vec4(light.specular, 1.0) * spec * color;

	float shadow = ShadowCalculation(fs_in.FragPosLightSpace, light, normal, lightDir);

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
			float bias = max(0.0003 * (1.0 - dot(normal, lightDir)), 0.00003);  
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}
	}

    return shadow / 9.0;
}  
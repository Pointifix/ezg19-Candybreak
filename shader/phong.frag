#version 460 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
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

const vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

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

    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.diffuse, TexCoords);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.diffuse, TexCoords);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.specular, TexCoords);

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
	if(projCoords.z > 1.0)
		return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
	float bias = 0.0003;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	float shadow = 1.0;

	for (int i = 0; i < 4; i++)
	{
		if (texture(shadowMap, projCoords.xy + poissonDisk[i] * texelSize).r > currentDepth - bias)
		{
			shadow -= 0.2;
		}
	}

    return shadow;
}  
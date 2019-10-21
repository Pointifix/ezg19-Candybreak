#version 460 core
out vec4 FragColor;

flat in int instanceID;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // phase 1: directional lighting
    vec4 result = CalcDirLight(dirLight, norm, viewDir);
    
	//if(instanceID % 3 == 0) result = result.rgba;
	//if(instanceID % 3 == 1) result = result.brga;
	//if(instanceID % 3 == 2) result = result.gbra;

    FragColor = result;
}

// calculates the color when using a directional light.
vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results

    vec4 ambient = vec4(light.ambient, 1.0) * texture(material.diffuse, TexCoords);
    vec4 diffuse = vec4(light.diffuse, 1.0) * diff * texture(material.diffuse, TexCoords);
    vec4 specular = vec4(light.specular, 1.0) * spec * texture(material.specular, TexCoords);

    return (ambient + diffuse + specular);
}
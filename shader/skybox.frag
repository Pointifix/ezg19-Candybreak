#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform float t;

uniform samplerCube skybox1;
uniform samplerCube skybox2;

const float startT = 0.15;
const float endT = 0.27;

void main()
{
	float localT = 0.0;
	
	if (t > endT) localT = 1.0;
	else if (t >= startT) localT = (t - startT) * (1.0 / (endT - startT));

    FragColor = mix(texture(skybox1, TexCoords), texture(skybox2, TexCoords), localT);
}
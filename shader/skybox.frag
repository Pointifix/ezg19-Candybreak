#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform float t;

uniform samplerCube skybox1;
uniform samplerCube skybox2;

void main()
{
    FragColor = mix(texture(skybox1, TexCoords), texture(skybox2, TexCoords), t);
	//FragColor = texture(skybox1, TexCoords);
}
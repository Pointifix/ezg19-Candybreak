#version 460 core
out vec4 FragColor;

in vec3 TexCoords;

uniform float t;

uniform samplerCube skybox1;
uniform samplerCube skybox2;

void main()
{
	float startT = 0.20;
	float endT = 0.27;
	bool subtract = false;

	if(t > 0.27)
	{
		startT = 0.37;
		endT = 0.44;
		subtract = true;
	}
	if(t > 0.44)
	{
		startT = 0.675;
		endT = 0.745;
		subtract = false;
	}

	float localT = 0.0;
	
	if (t > endT) localT = 1.0;
	else if (t >= startT) localT = (t - startT) * (1.0 / (endT - startT));

	if (subtract) localT = 1 - localT;
    FragColor = mix(texture(skybox1, TexCoords), texture(skybox2, TexCoords) * 0.5, localT);
}
#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float time;

uniform sampler2D aTexture;

uniform vec2 hitCoords[4];
uniform int hitCount;

void main()
{
	float alphaMultiplier = 0;

	for(int i = 0; i < hitCount; i++)
	{
		float distance = distance(TexCoords, hitCoords[i]);
		float falloff = (0.2 - distance) * 5;
		float tempMultiplier = pow(2, -(pow(distance - (time / 3.0), 2) * 4096)) * falloff;
		if(falloff > 0 && tempMultiplier > alphaMultiplier) alphaMultiplier = tempMultiplier;
	}

	FragColor = vec4(vec3(texture(aTexture, TexCoords)), alphaMultiplier);
}
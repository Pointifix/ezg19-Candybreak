#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform float time;

uniform sampler2D aTexture;

uniform vec3 hitCoords[4];
uniform int hitCount;

void main()
{
	float alphaMultiplier = 0.1;

	for(int i = 0; i < hitCount; i++)
	{
		float distance = distance(TexCoords, hitCoords[i].xy);
		float falloff = (0.2 - distance) * 5;
		float tempMultiplier = pow(2, -(pow(distance - (hitCoords[i].z / 3.0), 2) * 4096)) * falloff;
		if(falloff > 0 && tempMultiplier > alphaMultiplier) alphaMultiplier = tempMultiplier;
	}

	vec4 color = texture(aTexture, TexCoords);
	FragColor = vec4(color.rgb + (alphaMultiplier * vec3(0.0, 0.0, 20.0)), alphaMultiplier * color.a);
}
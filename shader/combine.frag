#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2DMS texture1;
uniform sampler2D texture2;

uniform int mode;

vec4 toneMapping(vec4 color)
{
	const float gamma = 2.2;
  
    vec4 mapped = color / (color + vec4(1.0));
    mapped = pow(mapped, vec4(1.0 / gamma));
  
    return mapped;
}

void main()
{
	vec4 firstSample;

	for (int i = 0; i < 4; i++)
	{
		firstSample += texelFetch(texture1, ivec2(TexCoords * textureSize(texture1)), i);
	}

	switch (mode)
	{
		case 0:
			FragColor = toneMapping(firstSample + texture(texture2, TexCoords));
			break;
		case 1:
			FragColor = toneMapping(firstSample * texture(texture2, TexCoords));
			break;
	}
}
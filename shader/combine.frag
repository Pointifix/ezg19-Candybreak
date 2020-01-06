#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform int mode;

vec4 exposureToneMapping(vec4 color)
{
	const float gamma = 2.2;
	const float exposure = 1.0;
  
    // Exposure tone mapping
    vec4 mapped = vec4(1.0) - exp(-color * exposure);
    // Gamma correction 
    mapped = pow(mapped, vec4(1.0 / gamma)); 
  
    return mapped;
}

void main()
{
	vec4 firstSample;

	for (int i = 0; i < 4; i++)
	{
		//firstSample += texelFetch(texture1, ivec2(TexCoords * textureSize(texture1)), i);
	}

	firstSample = texture(texture1, TexCoords);

	switch (mode)
	{
		case 0:
			FragColor = exposureToneMapping(firstSample + texture(texture2, TexCoords));
			break;
		case 1:
			FragColor = exposureToneMapping(firstSample * texture(texture2, TexCoords));
			break;
	}
}
#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform int mode;

vec4 toneMapping(vec4 color)
{
	const float gamma = 2.2;
	const float exposure = 5.0;

    vec4 mapped = vec4(1.0) - exp(-color * exposure);
	mapped = pow(mapped, vec4(1.0 / gamma));
    return mapped;
}

void main()
{
	switch (mode)
	{
		case 0:
			FragColor = texture(texture1, TexCoords) + texture(texture2, TexCoords);
			break;
		case 1:
			FragColor = texture(texture1, TexCoords) * texture(texture2, TexCoords);
			break;
		case 2:
			FragColor = toneMapping(texture(texture1, TexCoords) + texture(texture2, TexCoords));
			break;
	}
}